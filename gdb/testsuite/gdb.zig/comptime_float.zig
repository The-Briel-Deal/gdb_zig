const std = @import("std");
pub fn main() void {
    const a: comptime_float = 1.97;
    std.debug.print("a = {x}", .{a});
}
