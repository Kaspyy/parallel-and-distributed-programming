import java.util.Random;

class Thr extends Thread {

  private static final Random random = new Random();
  private int number;

  public Thr(int number) {
    this.number = number;
  }

  public void run() {
    for (int i = 1; i <= 5; i++) {
      int generate = random.nextInt(11);
      System.out.println(
        "Watek numer " + number + " wylosowal: " + generate + " po raz " + i
      );
    }
  }
}

public class Losthr {

  public static void main(String args[]) {
    int i;
    int num_threads = 10;

    Thr[] NewThr = new Thr[num_threads];

    for (i = 0; i < num_threads; i++) {
      (NewThr[i] = new Thr(i)).start();
    }

    for (i = 0; i < num_threads; i++) {
      try {
        NewThr[i].join();
      } catch (InterruptedException e) {}
    }
  }
}
