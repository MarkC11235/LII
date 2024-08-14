const Person = struct { // <- This const makes the Person variable immutable
    const name = "John"; // <- This const makes the name variable immutable
    let age = 30; // <- This let makes the age variable mutable
};

// const should work as expected for normal variables

// const vectors should not let you change the length of the vector or the values of the elements