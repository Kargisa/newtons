#pragma once

#include <fstream> 
#include <iostream> 
#include <vector>
#include <string>
#include <unordered_map>

namespace obj {
	struct Index {
		size_t vertex_index;
		size_t tex_coord_index;
		size_t normal_index;
	};

	struct Object {
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> tex_coords;
		std::vector<Index> indices;
	};

	std::vector<std::string> SplitString(const std::string& str, char delim) {
		std::vector<std::string> result;
		std::string current;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == delim) {
				result.emplace_back(current);
				current.clear();
			}
			else {
				current.push_back(str[i]);
			}
		}
		result.emplace_back(current);
		return result;
	}

	Object ReadObjFile(const char* path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open obj file");
		}

		std::string line;
		Object object;

		while (std::getline(file, line)) {
			if (line[0] == '#') {
				continue;
			}

			std::vector<std::string> parts = SplitString(line, ' ');
			std::string& type = parts[0];


			if (type == "v") {
				for (size_t i = 1; i < parts.size(); i++) {
					object.vertices.emplace_back(std::stof(parts[i]));
				}
			}
			else if (type == "vt") {
				for (size_t i = 1; i < parts.size(); i++) {
					object.tex_coords.emplace_back(std::stof(parts[i]));
				}
			}
			else if (type == "vn") {
				for (size_t i = 1; i < parts.size(); i++) {
					object.normals.emplace_back(std::stof(parts[i]));
				}
			}
			else if (type == "f") {
				for (size_t i = 1; i < parts.size(); i++) {
					std::vector<std::string> indices = SplitString(parts[i], '/');
					object.indices.emplace_back(
						static_cast<size_t>(std::stoi(indices[0])) - 1,
						static_cast<size_t>(std::stoi(indices[1])) - 1,
						static_cast<size_t>(std::stoi(indices[2])) - 1
					);
				}
			}
			else {
			}

		}
		return object;
	}
}