# Reading a file
## Code snippet
```cpp
// we don't want to put much code in a template method, thus we have this guy
std::tuple<std::ifstream, uintmax_t, bool>
open_file(string_view path)
{
    std::ifstream file(path.data(), std::ios::binary);
    if (!file) { return {std::ifstream(), uintmax_t(), false}; }

    /*
     * Getting a file size. This could be alternatively done with
     * std::filesystem::file_size() call but, this will work too.
     */
    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios::end);
    uintmax_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    return {std::move(file), file_size, true};
}

template <typename ContainerType>
std::optional<ContainerType>
read_file(string_view path)
{
    auto [file, size, ok] = open_file(path);
    if (!ok) { return {}; }

    ContainerType result;

    /*
     * Without reserving exact space prior to fill the container we risk to
     * over-allocate.
     */
    result.reserve(size);
    using value_type = typename ContainerType::value_type;
    result.insert(
        begin(result),
        std::istream_iterator<value_type>(file),
        std::istream_iterator<value_type>());
    return result;
}
```

## Usage
```cpp
int main()
{
    std::vector<uint8_t> v;
    constexpr char filename[] = "binary_hamster.bin";
    const auto container = read_file<decltype(v)>(filename);
    if (container) {
        v = std::move(*container);
    } else {
        std::cerr << "failed to read file " << filename << '\n';
        return EXIT_FAILURE;
    }

    for (const auto& byte : v) {
        std::cout << std::hex << static_cast<uint16_t>(byte);
    }
    std::cout << '\n';

    return EXIT_SUCCESS;
}
```
