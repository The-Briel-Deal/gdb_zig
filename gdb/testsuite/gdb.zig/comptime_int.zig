const std = @import("std");
pub fn main() void {
    const a: comptime_int = 13;
    std.debug.print("a = {x}", .{a});
}
