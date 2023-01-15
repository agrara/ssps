    <?php
    for ($i = 0; $i < 101; $i++) {
        if ($i % 15 == 0) {
            echo ("ThreeFive\n");
        } elseif ($i % 5 == 0) {
            echo ("Five\n");
        } elseif ($i % 3 == 0) {
            echo ("Three\n");
        } else {
            echo ($i . "\n");
        }
    }
    ?>