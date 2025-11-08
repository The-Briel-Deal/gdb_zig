pub fn main() void {
    const nums = [_]i32{ 2, 4, 6 };
    var sum: i32 = 0;
    inline for (nums) |i| {
        sum += i;
    }
}
