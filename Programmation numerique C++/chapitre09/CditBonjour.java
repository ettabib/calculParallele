// file CditBonjour.java
class CditBonjour {
    public native void ditBonjour();

    static {
        System.loadLibrary("bonjour");
    }

    public static void main(String[] args) {
        new CditBonjour().ditBonjour();
    }
}

