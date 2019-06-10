#include "Filesys.hpp"

namespace fs = std::filesystem;

std::vector<std::string> FilesInDir(const std::string& dir_name) {
	std::vector<std::string> result;
	fs::recursive_directory_iterator dir(dir_name);
	
	for (auto& f : dir) {
		if (fs::is_regular_file(f)) {
			result.push_back(f.path());
		}
	}

	return result;
}

std::vector<std::string> FilesInSubdirs(const std::string& base_dir, const std::vector<std::string>& sub_dirs) {
	std::vector<std::string> result;
	fs::directory_iterator dir(path);

	/*
	 *
	 * do this
	*/
}
