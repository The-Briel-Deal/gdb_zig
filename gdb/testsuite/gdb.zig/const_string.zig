pub fn main() void {
    const test_const_string: []const u8 = "foo bar baz";
    const test_string = "foo bar baz";
    _ = test_const_string;
    _ = test_string;
    return;
}
