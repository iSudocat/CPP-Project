#include <was/storage_account.h>
#include <was/table.h>
using namespace std;

int main() 
{

	// Define the Storage connection string with your values.
	const utility::string_t storage_connection_string(U("DefaultEndpointsProtocol=https;AccountName=sudopic;AccountKey=JrX9YGPa1R1WUy76UzpGnZjZELITmZOnhsSkSXfFwvwy7yH9IVAplVso7kKkeYVw9zgZq/4k4TgQbBQ8YJ5aTg==;EndpointSuffix=core.windows.net"));

	// Retrieve the storage account from the connection string.
	azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);

	// Create the table client.
	azure::storage::cloud_table_client table_client = storage_account.create_cloud_table_client();

	// Retrieve a reference to a table.
	azure::storage::cloud_table table = table_client.get_table_reference(U("people"));

	// Create the table if it doesn't exist.
	table.create_if_not_exists();

	// Create a new customer entity.
	azure::storage::table_entity customer1(U("Harp"), U("Walter"));

	azure::storage::table_entity::properties_type& properties = customer1.properties();
	properties.reserve(2);
	properties[U("Email")] = azure::storage::entity_property(U("Walter@contoso.com"));

	properties[U("Phone")] = azure::storage::entity_property(U("425-555-0101"));

	// Create the table operation that inserts the customer entity.
	azure::storage::table_operation insert_operation = azure::storage::table_operation::insert_entity(customer1);

	// Execute the insert operation.
	azure::storage::table_result insert_result = table.execute(insert_operation);

	return 0;
}

