//const Person = struct { // <- This const makes the Person variable immutable
//    const name = "John"; // <- This const makes the name variable immutable
//    let age = 30; // <- This let makes the age variable mutable
//};

// const should work as expected for normal variables

// const vectors should not let you change the length of the vector or the values of the elements


// let vec = $vector_create(10000, 0); // 11 ms

// let vec = [];  // 110941 ms

// for (let i = 0; i < 10000; i = i + 1) {
//     vec = $vector_push(vec, 0);
// }

