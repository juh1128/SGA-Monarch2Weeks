#include "stdafx.h"
#include "json.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

const int kReadBufferSize = 65536;


bool Json::read(std::string const& file) {
	errno_t err = ::_tfopen_s(&fp_, file.c_str(), _T("rb"));
	if (err) {
		return false;
	}

	char readBuffer[kReadBufferSize];
	FileReadStream readStream(fp_, readBuffer, _countof(readBuffer));
	bool result = !document_.ParseStream(readStream).HasParseError();

	::fclose(fp_);

	return result;
}

bool Json::write(std::string const& file) {
	errno_t err = ::_tfopen_s(&fp_, file.c_str(), _T("wb"));
	if (err) {
		return false;
	}

	char writeBuffer[kReadBufferSize];
	FileWriteStream writeStream(fp_, writeBuffer, _countof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(writeStream);
	document_.Accept(writer);

	::fclose(fp_);

	return true;
}

Value Json::makeStringValue(string str)
{
	char buffer[100];
	Value stringValue;
	int len = sprintf(buffer, "%s", str.c_str());
	stringValue.SetString(buffer, len, document_.GetAllocator());

	return stringValue;
}
