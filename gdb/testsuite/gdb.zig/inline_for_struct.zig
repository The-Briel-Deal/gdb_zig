const std = @import("std");

const FooStruct = struct {
    name: []const u8,
    num1: u32,
    num2: u32,
    sum: u32,
};

pub fn main() void {
    const test_cases = [_]FooStruct{
        .{ .name = "2 + 2 = 4", .num1 = 2, .num2 = 2, .sum = 4 },
        .{ .name = "5 + 6 = 11", .num1 = 5, .num2 = 6, .sum = 11 },
    };
    inline for (test_cases) |test_case| {
        const result_sum = test_case.num1 + test_case.num2;
        if (result_sum == test_case.sum) {
            std.debug.print("Case {s} passed!\n", .{test_case.name});
        } else {
            std.debug.print("Case {s} failed!\n", .{test_case.name});
        }
    }
}
