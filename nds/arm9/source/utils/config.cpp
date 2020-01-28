#include "config.hpp"
#include <nds/system.h>

#include "json.hpp"
#include "flashcard.hpp"
#include "lang.hpp"

nlohmann::json configJson;

void Config::load() {
	FILE* file = fopen(sdFound() ? "sd:/_nds/LeafEdit/config.json" : "fat:/_nds/LeafEdit/config.json", "r");
	if(file) {
		configJson = nlohmann::json::parse(file, nullptr, false);
		fclose(file);
	}
}

void Config::save() {
	FILE* file = fopen(sdFound() ? "sd:/_nds/LeafEdit/config.json" : "fat:/_nds/LeafEdit/config.json", "w");
	if(file) {
		fwrite(configJson.dump(1, '\t').c_str(), 1, configJson.dump(1, '\t').size(), file);
		fclose(file);
	}
}

bool Config::getBool(const std::string &key) {
	if(!configJson.contains(key)) {
		return false;
	}
	return configJson.at(key).get_ref<const bool&>();
}
void Config::setBool(const std::string &key, bool v) {
	configJson[key] = v;
}

int Config::getInt(const std::string &key) {
	if(!configJson.contains(key)) {
		return 0;
	}
	return configJson.at(key).get_ref<const int64_t&>();
}
void Config::setInt(const std::string &key, int v) {
	configJson[key] = v;
}

std::string Config::getString(const std::string &key) {
	if(!configJson.contains(key)) {
		return "";
	}
	return configJson.at(key).get_ref<const std::string&>();
}
void Config::setString(const std::string &key, const std::string &v) {
	configJson[key] = v;
}

int Config::getLang(const std::string &key) {
	return configJson.at(key).get_ref<const int64_t&>();
}
