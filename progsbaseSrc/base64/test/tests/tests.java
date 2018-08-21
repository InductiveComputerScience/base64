package tests;

import encodetests.testresult;

import static decodetests.decodetests.*;
import static encodetests.encodetests.*;

public class tests {
    public static double test(){
        double failures;
        testresult atestresult;
        failures = 0d;

        atestresult = testDecoder();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testDecoder2();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testdecoderWithPadding();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testDecoderWithPadding2();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testEncoder();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testEncoder2();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testEncoderWithPadding();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testEncoderWithPadding2();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        atestresult = testDecoderReal1();
        if(atestresult.failures != 0d){failures = failures + 1d;}

        return failures;
    }
}
