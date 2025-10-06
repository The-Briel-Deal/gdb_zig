const std = @import("std");
pub fn main() void {
    const a: comptime_int = 0xaabbccdd;
    std.debug.print("a = {x}", .{a});
}
