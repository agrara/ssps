for (let i = 0; i < 101; i++) {
    if (i % 15 == 0) {
        console.log("ThreeFive");
    } else if (i % 5 == 0) {
        console.log("Five");
    } else if (i % 3 == 0) {
        console.log("Three");
    } else {
        console.log(i)
    }
}