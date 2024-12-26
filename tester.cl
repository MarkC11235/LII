let point2 = struct{
    let x = 0;
    let y = 0;
};

let point_extended = struct{
    let z = 0;
    let q = 0;
};

let new_map = $map_extend(point2, point_extended);

print new_map;