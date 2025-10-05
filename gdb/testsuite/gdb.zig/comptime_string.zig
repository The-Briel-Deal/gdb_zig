pub fn main() void {
    const a = comptime "foo";
    const b = comptime "bar";
    const c = a ++ " " ++ b;

    _ = c;
    return;
}
