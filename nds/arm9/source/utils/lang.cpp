#include "lang.hpp"
#include "json.hpp"

std::vector<std::string> Lang::abilities, Lang::games, Lang::items, Lang::locations4, Lang::locations5, Lang::moves, Lang::natures, Lang::species;
const std::string langs[] = {"br", "de", "en", "es", "fr", "id", "it", "lt", "pt", "ru", "jp", "ko"};
nlohmann::json langJson;

void loadToVector(std::string path, std::vector<std::string> &vec) {
	char* line = NULL;
	size_t len = 0;
	vec.clear();

	FILE* in = fopen(path.c_str(), "r");
	if(in) {
		while(__getline(&line, &len, in) != -1) {
			if(line[strlen(line)-1] == '\n')	line[strlen(line)-1] = '\0';
			vec.push_back(line);
		}
		fclose(in);
	}
}

void Lang::load(int lang) {
	// Load app strings
	FILE* file = fopen(("nitro:/lang/"+langs[lang]+"/app.json").c_str(), "rt");
	if(file) {
		langJson = nlohmann::json::parse(file, nullptr, false);
		fclose(file);
	}
}

std::string Lang::get(const std::string &key) {
	if(!langJson.contains(key)) {
		return "MISSING: " + key;
	}
	return langJson.at(key).get_ref<const std::string&>();
}
