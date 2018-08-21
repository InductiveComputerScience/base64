package base64.Table;

import static java.lang.Math.*;

public class Table {
    public static char [] getTable(){
        return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".toCharArray();
    }

    public static double [] getRevTable(){
        char [] table;
        double i, max;
        double [] revTable;

        table = getTable();
        max = getMax(table) + 1d;
        revTable = new double [(int)(max + 1d)];

        for(i = 0d; i < table.length; i = i + 1d){
            revTable[(int)table[(int)i]] = i;
        }

        return revTable;
    }

    public static boolean [] getValidTable(){
        char [] table;
        double max, i;
        boolean [] validTable;

        table = getTable();
        max = getMax(table) + 1d;
        validTable = new boolean [(int)max];

        for(i = 0d; i < max; i = i + 1d){
            validTable[(int)i] = isValidBase64character((char)i);
        }

        return validTable;
    }

    public static double getMax(char[] table) {
        double maxValue;
        double v;
        double i;

        maxValue = 0d;

        for(i = 0d; i < table.length; i = i + 1d){
            v = table[(int)i];
            maxValue = max(maxValue, v);
        }
        return maxValue;
    }

    public static boolean isValidBase64character(char c) {
        char [] table;
        boolean isValid;
        double i;

        table = getTable();
        isValid = false;

        for(i = 0d; i < table.length; i = i + 1d){
            if(table[(int)i] == c){
                isValid = true;
            }
        }

        return isValid;
    }
}
