import java.util.Random;

class Thr extends Thread {
    private char[][] obraz;
    private char[] znaki;
    private int numerWatku;

    public Thr(int numerWatku, char[][] obraz, char znaki[]) {
        this.numerWatku = numerWatku;
        this.obraz = obraz;
        this.znaki = znaki;
    }

    public void run() {
        int liczbaZnakow = znaki.length;
        int[] liczba = new int[liczbaZnakow];
        int i, j;
        for (i = 0; i < obraz.length; i++) {
            for (j = 0; j < obraz[i].length; j++) {
                for (int k = 0; k < liczbaZnakow; k++) {
                    if (obraz[i][j] == znaki[k]) {
                        liczba[k]++;
                    }
                }
            }
        }
        for (i = 1; i < liczbaZnakow; i++) {
            System.out.println(
                    "Watek: " + numerWatku + "  " + znaki[i] + " " + liczba[i] + "x");
        }
    }
}

public class Histogram {
    public static void main(String[] args) {
        int n = 100;
        int m = 10;
        char[][] obraz = new char[n][m];

        // Fill obraz with random symbols
        Random random = new Random();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                obraz[i][j] = (char) (random.nextInt(26) + 'a');
            }
        }

        // create threads
        Thr[] watki = new Thr[3];
        watki[0] = new Thr(1, obraz, new char[] { 'a', 'b', 'c' });
        watki[1] = new Thr(2, obraz, new char[] { 'r', 'g', 't' });
        watki[2] = new Thr(3, obraz, new char[] { 's', 'h', 'f' });

        // start threads
        for (int i = 0; i < 3; i++) {
            watki[i].start();
        }

        // wait for threads to finish
        for (int i = 0; i < 3; i++) {
            try {
                watki[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

}
