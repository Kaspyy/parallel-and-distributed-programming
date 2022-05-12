import java.util.Random;

// Klasa wątku odpowiedzialna za obliczanie sumy wektorów
class VectorThread extends Thread {
    private int[] vector1;
    private int[] vector2;
    private int start;
    private int end;

    public VectorThread(int[] vector1, int[] vector2, int start, int end) {
        this.vector1 = vector1;
        this.vector2 = vector2;
        this.start = start;
        this.end = end;
    }

    public void run() {
        for (int i = start; i < end; i++) {
            vector2[i] = vector1[i] + vector2[i];
        }
    }
}

public class Vector {
    public static void main(String[] args) {
        int n = 10;
        int[] vector1 = new int[n];
        int[] vector2 = new int[n];
        Random random = new Random();

        // Inicjalizacja wektorów
        for (int i = 0; i < n; i++) {
            vector1[i] = random.nextInt(100);
            vector2[i] = random.nextInt(100);
        }

        int i;
        int numThreads = 3;

        VectorThread[] threads = new VectorThread[numThreads];

        // Utworzenie wątków
        for (i = 0; i < numThreads; i++) {
            threads[i] = new VectorThread(vector1, vector2, i * n / numThreads, (i + 1) * n / numThreads);
        }

        // Uruchomienie wątków
        for (i = 0; i < numThreads; i++) {
            threads[i].start();
        }

        // Oczekiwanie na zakończenie wątków
        for (i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // Wypisanie wyniku
        for (i = 0; i < n; i++) {
            System.out.println(vector2[i]);
        }

        System.out.println("Done");
    }

}
