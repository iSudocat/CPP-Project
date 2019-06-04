#include <iostream>
#include <speechapi_cxx.h>
#include <fstream>
#include "wav_file_reader.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;


std::wstring utf8string2wstring(const std::string& str)//编码转换
{
	static std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.from_bytes(str);
}


wstring SpeechRecognition() //WithLanguageAndUsingDetailedOutputFormat
{
	// Creates an instance of a speech config with specified subscription key and service region.
	// Replace with your own subscription key and service region (e.g., "westus").
	auto config = SpeechConfig::FromSubscription("8cf2551a4fed4845b08e37a08f920fb5", "eastasia");

	// Creates a speech recognizer in the specified language using microphone as audio input.
	// Replace the language with your language in BCP-47 format, e.g. en-US.
	auto lang = "zh-CN";
	config->SetSpeechRecognitionLanguage(lang);
	// Request detailed output format.
	config->SetOutputFormat(OutputFormat::Detailed);

	auto recognizer = SpeechRecognizer::FromConfig(config);
	cout << "Say something in " << lang << "...\n";

	// Starts speech recognition, and returns after a single utterance is recognized. The end of a
	// single utterance is determined by listening for silence at the end or until a maximum of 15
	// seconds of audio is processed.  The task returns the recognition text as result. 
	// Note: Since RecognizeOnceAsync() returns only a single utterance, it is suitable only for single
	// shot recognition like command or query. 
	// For long-running multi-utterance recognition, use StartContinuousRecognitionAsync() instead.
	auto result = recognizer->RecognizeOnceAsync().get();

	// Checks result.
	if (result->Reason == ResultReason::RecognizedSpeech)
	{
		wstring s = utf8string2wstring(result->Text);
		wcout.imbue(std::locale(std::locale("Chinese"), new std::codecvt_byname<wchar_t, char, std::mbstate_t>("Chinese")));
		std::wcout << "RECOGNIZED: Text=" << s << endl;
			//<< "  Speech Service JSON: " << utf8string2wstring(result->Properties.GetProperty(PropertyId::SpeechServiceResponse_JsonResult)) << std::endl;
		return s;
/*
string::size_type idx=s.find(L"左");
		if (idx == string::npos)//不存在。
			cout << "not found\n";
		else//存在。
			//cout << "found\n";
			//return 1;
			DoLeftShift(hdc);
*/
		
	}
	else if (result->Reason == ResultReason::NoMatch)
	{
		cout << "NOMATCH: Speech could not be recognized." << std::endl;
	}
	else if (result->Reason == ResultReason::Canceled)
	{
		auto cancellation = CancellationDetails::FromResult(result);
		cout << "CANCELED: Reason=" << (int)cancellation->Reason << std::endl;

		if (cancellation->Reason == CancellationReason::Error)
		{
			cout << "CANCELED: ErrorCode=" << (int)cancellation->ErrorCode << std::endl;
			cout << "CANCELED: ErrorDetails=" << cancellation->ErrorDetails << std::endl;
			cout << "CANCELED: Did you update the subscription info?" << std::endl;
		}
	}
}

