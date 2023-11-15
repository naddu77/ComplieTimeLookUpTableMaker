# ComplieTimeLookUpTableMaker
```C++
import std;

template <auto length, std::invocable<decltype(length)> Generator>
consteval auto MakeLookUpTable(Generator&& generator)
{
    std::array<std::invoke_result_t<Generator, decltype(length)>, length> look_up_table;

    std::ranges::copy(
        std::views::iota(decltype(length){}, length) | std::views::transform(std::forward<Generator>(generator)),
        std::begin(look_up_table)
    );

    return look_up_table;
}

int main()
{
    constexpr auto lookup_table{ MakeLookUpTable<0xFF>([](char c) {
        return 'A' <= c and c <= 'Z' ? static_cast<char>(c + 0x20) : c;
    }) };

    static_assert(lookup_table['a'] == lookup_table['A']);
    static_assert(lookup_table['z'] == lookup_table['Z']);
    static_assert(lookup_table['-'] == lookup_table['-']);
    static_assert(lookup_table['{'] == lookup_table['{']);
    static_assert(lookup_table['}'] == lookup_table['}']);
    static_assert(lookup_table['a'] != lookup_table['b']);

    for (auto [i, c] : lookup_table | std::views::enumerate | std::views::take(128))
    {
        if (std::iswcntrl(c))
        {
            std::println("{0}(0x{0:X}) = not printable", i, c);
        }
        else
        {
            std::println("{0}(0x{0:X}) = {1}", i, c);
        }
    }
}
```
