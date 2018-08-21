package decodetests;

import base64.dataStructures.Decoded;
import base64.dataStructures.Encoded;
import encodetests.string;
import encodetests.testresult;

import static base64.Decoder.Decoder.decode;
import static base64.Decoder.Decoder.decode3;
import static base64.Encoder.Encoder.encode;
import static encodetests.encodetests.stringFrom;
import static java.lang.Math.*;
import static arrays.arrays.arrays.*;

public class decodetests {

    public static testresult testDecoder() {
        char [] orginal, result;
        double[] data;
        Encoded encoded;
        Decoded decoded;
        testresult atestresult;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string[1];

        orginal = "Hei".toCharArray();
        data = stringToNumberArray(orginal);
        encoded = encode(data);

        decoded = decode3(encoded.data);

        result = numberArrayToString(decoded.data);

        if(!stringsEqual(orginal, result)){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!decoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testDecoder2() {
        testresult atestresult;
        double [] original;
        Encoded encoded;
        Decoded decoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        original = new double [3];
        original[0] = 0d;
        original[1] = pow(2d, 4d);
        original[2] = pow(2d, 8d) - 1d;
        encoded = encode(original);

        decoded = decode3(encoded.data);

        if(!numberArraysEqual(original, decoded.data)){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!decoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testdecoderWithPadding() {
        testresult atestresult;
        char [] original;
        double[] data;
        Encoded encoded;
        Decoded decoded;
        char [] result;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        original = "Hei12".toCharArray();
        data = stringToNumberArray(original);
        encoded = encode(data);

        decoded = decode(encoded.data);

        result = numberArrayToString(decoded.data);

        if(!stringsEqual(original, result)){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!decoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testDecoderWithPadding2() {
        testresult atestresult;
        char [] original;
        double[] data;
        Encoded encoded;
        Decoded decoded;
        char [] result;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        original = "Hei1".toCharArray();
        data = stringToNumberArray(original);
        encoded = encode(data);

        decoded = decode(encoded.data);

        result = numberArrayToString(decoded.data);

        if(!stringsEqual(original, result)){
            atestresult.failures = atestresult.failures + 1d;
        }

        if(!decoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
        }

        return atestresult;
    }

    public static testresult testDecoderReal1() {
        testresult atestresult;
        Decoded decoded;

        atestresult = new testresult();
        atestresult.failures = 0d;
        atestresult.errorMessages = new string [1];

        decoded = decode("56Lw4SQJdBIDNUoc+QNvbuQQo/osGyGQzfLfluGLJ+g=".toCharArray());

        if(!decoded.success){
            atestresult.failures = atestresult.failures + 1d;
            atestresult.errorMessages[0] = stringFrom(decoded.errorMessage);
        }

        return atestresult;
    }
}
