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
    constexpr auto generator{ [](auto e) { return e == '{' or e == '}'; } };
    constexpr auto look_up_table{ MakeLookUpTable<128>(generator) };

    static_assert(look_up_table['{']);
    static_assert(look_up_table['}']);
    static_assert(!look_up_table['-']);

    for (auto [i, c] : look_up_table | std::views::enumerate)
    {
        std::println("{:c} = {}", i, c);
    }
}
