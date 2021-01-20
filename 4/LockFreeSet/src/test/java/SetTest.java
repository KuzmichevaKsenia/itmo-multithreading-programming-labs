import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

public class SetTest {
    private final int iterCount = 10;

    @Test
    public void add() throws InterruptedException {
        Set<Integer> set = new SetImpl<>();
        List<Thread> threads = new ArrayList<>();

        for (int i = 1; i < iterCount; i++) {
            int finalI = i;
            threads.add(new Thread(() -> assertTrue(set.add(finalI))));
        }

        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.join();
        }
    }

    @Test
    public void remove() throws InterruptedException {
        Set<Integer> set = new SetImpl<>();
        List<Thread> threads = new ArrayList<>();

        for (int i = 1; i < iterCount; i++) {
            set.add(i);
        }

        for (int i = 1; i < iterCount; i++) {
            int finalI = i;
            threads.add(new Thread(() -> assertTrue(set.remove(finalI))));
        }

        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.join();
        }
    }

    @Test
    public void contains() throws InterruptedException {
        Set<Integer> set = new SetImpl<>();
        List<Thread> threads = new ArrayList<>();

        for (int i = 1; i < iterCount; i++) {
            set.add(i);
        }

        for (int i = 1; i < iterCount; i++) {
            int finalI = i;
            threads.add(new Thread(() -> {
                assertTrue(set.contains(finalI));
                assertFalse(set.contains(finalI + iterCount));
            }));
        }

        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.join();
        }
    }

    @Test
    public void isEmpty() throws InterruptedException {
        Set<Integer> set = new SetImpl<>();
        List<Thread> threads = new ArrayList<>();

        for (int i = 1; i < iterCount; i++) {
            threads.add(new Thread(() -> assertTrue(set.isEmpty())));
        }

        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.join();
        }

        set.add(1);

        threads = new ArrayList<>();

        for (int i = 1; i < iterCount; i++) {
            threads.add(new Thread(() -> assertFalse(set.isEmpty())));
        }

        for (Thread thread : threads) {
            thread.start();
        }
        for (Thread thread : threads) {
            thread.join();
        }

    }
}
