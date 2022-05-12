class ErathostenesThread extends Thread {
    private int max;
    private int[] primes;
    private int start;
    private int end;

    public ErathostenesThread(int max, int[] primes, int start, int end) { // Konstruktor
        this.max = max;
        this.primes = primes;
        this.start = start;
        this.end = end;
    }

    public void run() {
        for (int i = start; i < end; i++) {
            if (primes[i] == 1) {
                for (int j = i * i; j < max; j += i) {
                    primes[j] = 0;
                }
            }
        }
    }
}

public class Erathostenes {
    public static void main(String[] args) {
        int max = 100; // maksymalna liczba
        int[] primes = new int[max]; // tablica z liczbami pierwszymi

        for (int i = 0; i < max; i++) {
            primes[i] = 1;
        }

        primes[0] = 0; // 0 nie jest pierwsza
        primes[1] = 0; // 1 nie jest pierwsza

        int numThreads = 4; // liczba wątków

        ErathostenesThread[] threads = new ErathostenesThread[numThreads]; // tablica wątków

        int chunk = max / numThreads; // dzielenie liczby na części

        for (int i = 0; i < numThreads; i++) { // tworzenie wątków
            threads[i] = new ErathostenesThread(max, primes, i * chunk, (i + 1) * chunk);
            threads[i].start();
        }
        for (int i = 0; i < numThreads; i++) { // czekanie na wątki
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        for (int i = 0; i < max; i++) { // wypisanie wyników
            if (primes[i] == 1) {
                System.out.println(i);
            }
        }
    }
}