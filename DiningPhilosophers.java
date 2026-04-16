
import java.time.Duration;
import java.time.Instant;
import java.util.concurrent.locks.ReentrantLock;

class Fork {

    private final int id;
    private final ReentrantLock lock;

    public Fork(int id) {
        this.id = id;
        this.lock = new ReentrantLock(true); // fair lock
    }

    public void pickUp(int philosopherId, String side) {
        lock.lock();
        System.out.println(TimeStamp.get() + " Philosopher "
                + philosopherId + " picked up " + side + " Fork.");
    }

    public void putDown(int philosopherId, String side) {
        lock.unlock();
        System.out.println(TimeStamp.get() + " Philosopher "
                + philosopherId + " put down " + side + " Fork.");
    }

    public int getId() {
        return id;
    }
}

class Philosopher extends Thread {

    private final int id;
    private final Fork leftFork;
    private final Fork rightFork;

    public Philosopher(int id, Fork leftFork, Fork rightFork) {
        this.id = id;
        this.leftFork = leftFork;
        this.rightFork = rightFork;
    }

    private void think() throws InterruptedException {
        System.out.println(TimeStamp.get()
                + " Philosopher " + id + " is Thinking.");
        Thread.sleep(1000);
    }

    private void eat() throws InterruptedException {

        // Resource hierarchy: pick lower ID fork first
        Fork first = (leftFork.getId() < rightFork.getId())
                ? leftFork : rightFork;
        Fork second = (leftFork.getId() < rightFork.getId())
                ? rightFork : leftFork;

        String firstSide = (first == leftFork) ? "Left" : "Right";
        String secondSide = (second == leftFork) ? "Left" : "Right";

        first.pickUp(id, firstSide);
        second.pickUp(id, secondSide);

        System.out.println(TimeStamp.get()
                + " Philosopher " + id + " is Eating.");

        Thread.sleep(2000);

        second.putDown(id, secondSide);
        first.putDown(id, firstSide);
    }

    @Override
    public void run() {
        try {
            while (true) {
                think();
                eat();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class TimeStamp {

    private static final Instant start = Instant.now();

    public static String get() {
        long seconds = Duration.between(start, Instant.now()).getSeconds();
        long minutes = seconds / 60;
        long remainingSeconds = seconds % 60;
        return String.format("[%02d:%02d]", minutes, remainingSeconds);
    }
}

public class DiningPhilosophers {

    public static void main(String[] args) {

        final int NUM_PHILOSOPHERS = 5;

        Fork[] forks = new Fork[NUM_PHILOSOPHERS];
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            forks[i] = new Fork(i);
        }

        Philosopher[] philosophers = new Philosopher[NUM_PHILOSOPHERS];

        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            Fork left = forks[i];
            Fork right = forks[(i + 1) % NUM_PHILOSOPHERS];
            philosophers[i] = new Philosopher(i + 1, left, right);
            philosophers[i].start();
        }
    }
}
