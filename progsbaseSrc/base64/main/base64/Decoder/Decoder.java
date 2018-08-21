package base64.Decoder;

import base64.dataStructures.Decoded;

import static base64.Table.Table.getRevTable;
import static base64.Table.Table.getValidTable;
import static java.lang.Math.*;

public class Decoder {
    public static Decoded decode(char data[]) {
        boolean done;
        char [] data4;
        double quads, i, currentQuad, currentTriplet, padding;
        Decoded decoded3, decoded;
        double [] table;
        boolean [] validTable;

        decoded = new Decoded();
        table = getRevTable(); // Alloc 1
        validTable = getValidTable(); // Alloc 3

        padding = 0d;
        if(data.length > 1d){
            if(data[(int)(data.length - 1d)] == '=') {
                padding = padding + 1d;
            }
        }
        // TODO: Evaluate && left to right and stop if false?
        if(data.length > 2d){
            if(data[(int)(data.length - 2d)] == '=') {
                padding = padding + 1d;
            }
        }
        quads = data.length / 4d;

        // TODO: Require init?
        // Init
        decoded.data = new double [(int)(quads*3d - padding)];
        decoded.errorMessage = new char [0];
        decoded.success = true;

        done = false;
        for(i = 0d; i < quads && !done; i = i + 1d){
            // TODO: How to differentiate between stack and heap allocation?
            data4 = new char [4];
            currentQuad = i * 4d;
            currentTriplet = i * 3d;

            if(padding > 0d && (i + 1d) == quads){
                if(padding == 2d){
                    data4[0] = data[(int)(currentQuad + 0d)];
                    data4[1] = data[(int)(currentQuad + 1d)];
                    data4[2] = 'A';
                    data4[3] = 'A';
                }else if(padding == 1d){
                    data4[0] = data[(int)(currentQuad + 0d)];
                    data4[1] = data[(int)(currentQuad + 1d)];
                    data4[2] = data[(int)(currentQuad + 2d)];
                    data4[3] = 'A';
                }
            }else{
                data4[0] = data[(int)(currentQuad + 0d)];
                data4[1] = data[(int)(currentQuad + 1d)];
                data4[2] = data[(int)(currentQuad + 2d)];
                data4[3] = data[(int)(currentQuad + 3d)];
            }

            decoded3 = decode3table(data4, table, validTable); // Alloc 2

            if(decoded3.success) {
                if(padding > 0d && i + 1d == quads){
                    if(padding == 2d){
                        decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
                    }else if(padding == 1d){
                        decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
                        decoded.data[(int)(currentTriplet + 1d)] = decoded3.data[1];
                    }
                }else {
                    decoded.data[(int)(currentTriplet + 0d)] = decoded3.data[0];
                    decoded.data[(int)(currentTriplet + 1d)] = decoded3.data[1];
                    decoded.data[(int)(currentTriplet + 2d)] = decoded3.data[2];
                }
            }else{
                decoded.success = false;
                decoded.errorMessage = decoded3.errorMessage;
                done = true;
            }

            delete(decoded3); // Unalloc 2
        }

        delete(table); // Unalloc 1
        return decoded;
    }

    public static void delete(Object object) {
        // Java has garbage collection.
    }

    public static Decoded decode3(char data[]) {
        double [] table;
        boolean [] validTable;
        Decoded decoded;

        table = getRevTable(); // Alloc 1
        validTable = getValidTable(); // Alloc 2

        decoded = decode3table(data, table, validTable);

        delete(validTable); // Unalloc 2
        delete(table); // Unalloc 1

        return decoded;
    }

    public static Decoded decode3table(char data[], double [] table, boolean [] validTable) {
        double i, valid;
        Decoded decoded;

        decoded = new Decoded(); // Alloc r

        decoded.success = false;
        decoded.data = new double [3];
        decoded.errorMessage = "".toCharArray();

        if(data.length == 4d){
            valid = 0d;

            for(i = 0d; i < 4d; i = i + 1d){
                if(isValidBase64characterTable(data[(int)i], validTable)){
                    valid = valid + 1d;
                }
            }

            if(valid == 4d){
                decode3NoChecksTable(decoded, data, table);
                decoded.success = true;
            }else{
                decoded.errorMessage = "Invalid character in input string.".toCharArray();
                decoded.success = false;
            }
        }else{
            decoded.errorMessage = "There must be exactly four characters in the input string.".toCharArray();
            decoded.success = false;
        }

        return decoded;
    }

    public static boolean isValidBase64characterTable(char c, boolean [] validTable) {
        return validTable[c];
    }

    public static void decode3NoChecks(Decoded decoded, char[] data) {
        double [] table;

        table = getRevTable();
        decode3NoChecksTable(decoded, data, table);
    }

    public static void decode3NoChecksTable(Decoded decoded, char[] data, double [] table) {
        double total, i, n, r;

        total = 0d;
        for(i = 0d; i < 4d; i = i + 1d){
            n = getNumber(data[(int)(4d - i - 1d)], table);

            total = total + n * pow(2d, i*6d);
        }

        for(i = 0d; i < 3d; i = i + 1d){
            r = total % pow(2d, 8d);
            decoded.data[(int)(3d - i - 1d)] = r;

            // Compact form
            total = (total - r)/pow(2d, 8d);
            /* Expanded form */
            /*{
              double t1, t2;
              t1 = total - r;
              t2 = pow(2d, 8d);
              total = t1 / t2;
            }*/
        }
    }

    public static double getNumber(char c, double [] table) {
        return table[(int)c];
    }
}
