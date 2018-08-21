package encodetests;

import base64.dataStructures.Encoded;

import static base64.Encoder.Encoder.encode;
import static base64.Encoder.Encoder.encode3;
import static java.lang.Math.*;
import static arrays.arrays.arrays.stringsEqual;
import static arrays.arrays.arrays.stringToNumberArray;

public class encodetests {

    public static testresult testEncoder() {
        testresult atestresult;
        char [] hei;
        double [] data;
        Encoded encoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [0];

        hei = "Hei".toCharArray();

        data = stringToNumberArray(hei);

        encoded = encode3(data);

        if(!stringsEqual(encoded.data, "SGVp".toCharArray())){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!encoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
        }

        return atestresult;
    }

    public static string stringFrom(char[] src) {
        string str;
        str = new string();
        str.str = src;
        return str;
    }

    public static testresult testEncoder2() {
        testresult atestresult;
        double data [];
        Encoded encoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        // TODO: Support direct instantiation.
        data = new double [3];
        data[0] = 0d;
        data[1] = pow(2d, 4d);
        data[2] = pow(2d, 8d) - 1d;

        encoded = encode3(data);

        if(!stringsEqual(encoded.data, "ABD/".toCharArray())){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!encoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testEncoderWithPadding() {
        testresult atestresult;
        char [] hei;
        double[] data;
        Encoded encoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        hei = "Hei12".toCharArray();

        data = stringToNumberArray(hei);

        encoded = encode(data);

        if(!stringsEqual(encoded.data, "SGVpMTI=".toCharArray())){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!encoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testEncoderWithPadding2() {
        testresult atestresult;
        char [] hei;
        double[] data;
        Encoded encoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        hei = "Hei1".toCharArray();

        data = stringToNumberArray(hei);

        encoded = encode(data);

        if(!stringsEqual(encoded.data, "SGVpMQ==".toCharArray())){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!encoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(encoded.errorMessage);
        }

        return atestresult;
    }
}
