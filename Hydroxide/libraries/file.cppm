module;

#include <string>
#include <fstream>
#include <filesystem>

export module file;

export namespace file {
	enum class format {
		text,
		binary
	};

	template<format format = format::text, typename T>
	std::string read(const T& path) {
		if constexpr (format == format::text) {
			std::ifstream t(path);
			std::string data((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			t.close();
			return data;
		}
		else if constexpr (format == format::binary) {
			std::ifstream t(path, std::ios::binary);
			std::string data((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			t.close();
			return data;
		}
	}

	template<format format = format::text, typename T>
	void write(const T& path, const std::string& data) {
		if constexpr (format == format::text) {
			std::ofstream t(path);
			t << data;
			t.close();
		}
		else if constexpr (format == format::binary) {
			std::ofstream t(path, std::ios::binary);
			t << data;
			t.close();
		}
	}
}
