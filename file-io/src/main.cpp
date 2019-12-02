#ifdef WRITE_FILE
#include <array>
#endif // WRITE_FILE

#include <cstdlib> // EXIT_SUCCESS and EXIT_FAILURE

#ifdef WITH_FILESYSTEM
#include <filesystem> // for std::filesystem's path and file_size
#endif                // WITH_FILESYSTEM

#include <fstream>
#include <iostream>
#include <iterator> // std::istream_iterator
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#include "binary_dir.hpp"

namespace {

#ifdef WITH_FILESYSTEM
namespace fs = std::filesystem;
#endif // WITH_FILESYSTEM

using std::ifstream;
using std::istream_iterator;
using std::string_view;

std::tuple<ifstream, uintmax_t, bool>
open_file(string_view path)
{
    ifstream file(path.data(), std::ios::binary);
    if (!file) { return {ifstream(), uintmax_t(), false}; }
    file.unsetf(std::ios::skipws);

    uintmax_t file_size;
#ifdef WITH_FILESYSTEM
    file_size = fs::file_size(path);
#else  // NOT WITH_FILESYSTEM
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
#endif // WITH_FILESYSTEM

    return {std::move(file), file_size, true};
}

template <typename ContainerType>
std::optional<ContainerType>
read_file(string_view path)
{
    // move opening the file and getting it's size out of template function
    auto [file, size, ok] = open_file(path);
    if (!ok) { return {}; }

    ContainerType result;
    result.reserve(size);
    using value_type = typename ContainerType::value_type;
    result.insert(
        begin(result),
        istream_iterator<value_type>(file),
        istream_iterator<value_type>());
    return result;
}

} // namespace

int
main(int argc, char** argv)
{
    constexpr auto    default_path = BINARY_DIR "binaryfile.bin";
    const std::string path         = argc == 2 ? argv[1] : default_path;

#ifdef WRITE_FILE
    std::ofstream file(default_path, std::ios::binary);
    if (!file) { return EXIT_FAILURE; }
    constexpr std::array<uint8_t, 4> data = {0xde, 0xad, 0xbe, 0xef};
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));
    file.close();
#else
    std::vector<uint8_t> v;
    const auto           container = read_file<decltype(v)>(path);
    if (container) {
        v = std::move(*container);
    } else {
        std::cerr << "failed to read file\n";
        return EXIT_FAILURE;
    }

    for (const auto& element : v) {
        std::cout << std::hex << static_cast<uint16_t>(element);
    }
    std::cout << '\n';
#endif // WRITE_FILE

    return EXIT_SUCCESS;
}
