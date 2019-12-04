# Reading a file
## Code snippet
```cpp
// we don't want to put much code in a template method, thus we have this guy
std::tuple<std::ifstream, uintmax_t, bool>
open_file(string_view path)
{
    // open file at the end to get size of it
    std::ifstream file(path.data(), std::ios::ate | std::ios::binary);
    if (!file) { return {std::ifstream(), uintmax_t(), false}; }
    
    uintmax_t file_size = file.tellg();
    file.seekg(0, std::ios::beg); // go back to beginning
    file.unsetf(std::ios::skipws); // don't skip eols, please
    
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
    if (!container) {
        std::cerr << "failed to read file " << filename << '\n';
        return EXIT_FAILURE;    
    }

    v = std::move(*container);
    for (const auto& byte : v) {
        std::cout << std::hex << static_cast<uint16_t>(byte);
    }
    std::cout << '\n';

    return EXIT_SUCCESS;
}
```
## Side notes

You could use `std::filesystem::file_size` to get file size. But decided not to do it, since it'll bloat the size of binary in my case.
