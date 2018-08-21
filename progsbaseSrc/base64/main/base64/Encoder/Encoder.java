package base64.Encoder;

import base64.dataStructures.Encoded;

import static base64.Table.Table.getTable;
import static java.lang.Math.*;

public class Encoder {
    // encoder
    public static Encoded encode(double [] data){
        Encoded encoded, encoded3;
        double padding, triplets, i, currentTriplet, currentQuad;
        double [] data3;
        boolean done;

        encoded = new Encoded();

        padding = 0d;
        if((data.length % 3d) == 1d){
            padding = 2d;
        }
        if((data.length % 3d) == 2d){
            padding = 1d;
        }
        triplets = ceil(data.length / 3d);

        // Init
        encoded.data = new char [(int)(triplets*4d)];
        encoded.errorMessage = new char [0];
        encoded.success = true;

        done = false;
        for(i = 0d; i < triplets && !done; i = i + 1d){
            data3 = new double [3];
            currentTriplet = i * 3d;
            currentQuad = i * 4d;

            if(padding > 0d && i + 1d == triplets){
                if(padding == 2d){
                    data3[0] = data[(int)(currentTriplet + 0d)];
                    data3[1] = 0d;
                    data3[2] = 0d;
                }else if(padding == 1d){
                    data3[0] = data[(int)(currentTriplet + 0d)];
                    data3[1] = data[(int)(currentTriplet + 1d)];
                    data3[2] = 0d;
                }
            }else{
                data3[0] = data[(int)(currentTriplet + 0d)];
                data3[1] = data[(int)(currentTriplet + 1d)];
                data3[2] = data[(int)(currentTriplet + 2d)];
            }

            encoded3 = encode3(data3);

            if(encoded3.success) {
                encoded.data[(int)(currentQuad + 0d)] = encoded3.data[0];
                encoded.data[(int)(currentQuad + 1d)] = encoded3.data[1];

                if(padding > 0d && i + 1d == triplets){
                    if(padding == 2d){
                        encoded.data[(int)(currentQuad + 2d)] = '=';
                        encoded.data[(int)(currentQuad + 3d)] = '=';
                    }else if(padding == 1d){
                        encoded.data[(int)(currentQuad + 2d)] = encoded3.data[2];
                        encoded.data[(int)(currentQuad + 3d)] = '=';
                    }
                }else {
                    encoded.data[(int)(currentQuad + 2d)] = encoded3.data[2];
                    encoded.data[(int)(currentQuad + 3d)] = encoded3.data[3];
                }
            }else{
                encoded.success = false;
                encoded.errorMessage = encoded3.errorMessage;
                done = true;
            }

            delete(encoded3);
        }

        return encoded;
    }

    public static void delete(Object object) {
        // Java has garbage collection.
    }

    public static Encoded encode3(double [] data){
        Encoded encoded;
        double elementsVerified, i, e;
        boolean isWithinBounds, isWhole;

        // TODO: How to enforce initialization?
        encoded = new Encoded();
        // Init
        encoded.data = new char [4];
        encoded.data[0] = 'A';
        encoded.data[1] = 'A';
        encoded.data[2] = 'A';
        encoded.data[3] = 'A';
        encoded.errorMessage = new char [0];
        encoded.success = false;

        // Check.
        if(data.length == 3d){
            elementsVerified = 0d;
            for(i = 0d; i < data.length; i = i + 1d){
                e = data[(int)(i)];

                // TODO: Check % for doubles for e.g. C: use fmod in C and C++.
                isWithinBounds = (e >= 0d) && (e < pow(2d, 8d));
                isWhole = (e % 1d) == 0d;
                if(isWithinBounds && isWhole){
                    elementsVerified = elementsVerified + 1d;
                }else{
                    encoded.errorMessage = "Input number is too high, too low or is not a whole number.".toCharArray();
                    encoded.success = false;
                }
            }

            if(elementsVerified == data.length){
                encode3NoChecks(encoded, data);
                encoded.success = true;
            }
        }else{
            encoded.errorMessage = "Input must contain 3 numbers.".toCharArray();
            encoded.success = false;
        }

        return encoded;
    }

    public static void encode3NoChecks(Encoded encoded, double[] data) {
        double total, i, bit6;
        char c;

        total = 0d;
        for(i = 0d; i < data.length; i = i + 1d){
            total = total + data[(int)(data.length - i - 1d)] * pow(2d, i*8d);
        }

        for(i = 0d; i < 4d; i = i + 1d){
            bit6 = total % pow(2d, 6d);
            total = (total - bit6) / pow(2d, 6d);

            c = getCharacter(bit6);

            encoded.data[(int)(4d - i - 1d)] = c;
        }
    }

    public static char getCharacter(double bit6) {
        char [] table;

        table = getTable();

        return table[(int)bit6];
    }
}
