import java.util.concurrent.atomic.AtomicMarkableReference;

public class SetImpl<T extends Comparable<T>> implements Set<T> {
    private final Node<T> tail = new Node<>(null, null);
    private final Node<T> head = new Node<>(null, tail);

    private static class Node<T extends Comparable<T>> {
        private final T value;
        private final AtomicMarkableReference<Node<T>> next;

        Node(T value, Node<T> next) {
            this.value = value;
            this.next = new AtomicMarkableReference<>(next, false);
        }
    }

    private static class Pair<T extends Comparable<T>> {
        private final Node<T> leftNode;
        private final Node<T> rightNode;

        Pair(Node<T> leftNode, Node<T> rightNode) {
            this.leftNode = leftNode;
            this.rightNode = rightNode;
        }
    }

    private <E extends Comparable<E>> int compare(Node<E> cur, E value) {
        return cur == tail ? 1 : cur.value.compareTo(value);
    }

    private Pair<T> find(T value) {
        find_again:
        while (true) {
            Node<T> leftNode = head;
            Node<T> rightNode = head.next.getReference();
            while (rightNode != null) {
                boolean[] marked = {false};
                Node<T> next = rightNode.next.get(marked);

                while (marked[0]) {
                    if (leftNode.next.compareAndSet(rightNode, next, false, false)) {
                        continue find_again;
                    }
                    rightNode = next;
                    next = rightNode.next.get(marked);
                }

                if (compare(rightNode, value) >= 0) {
                    return new Pair<>(leftNode, rightNode);
                }
                leftNode = rightNode;
                rightNode = next;
            }
        }
    }

    @Override
    public boolean add(T value) {
        Node<T> newNode = new Node<>(value, null);
        while (true) {
            Pair<T> pos = find(value);
            if ((pos.rightNode != tail) && (pos.rightNode.value.compareTo(value) == 0)) {
                return false;
            }
            newNode.next.set(pos.rightNode, false);

            if (pos.leftNode.next.compareAndSet(pos.rightNode, newNode, false, false)) {
                return true;
            }
        }
    }

    @Override
    public boolean remove(T value) {
        while (true) {
            Pair<T> FondedPos = find(value);
            if ((FondedPos.rightNode == tail) || (FondedPos.rightNode.value.compareTo(value) != 0)) {
                return false;
            }

            Node<T> Neighbor = FondedPos.rightNode.next.getReference();
            if (FondedPos.rightNode.next.attemptMark(Neighbor, true)) {
                FondedPos.leftNode.next.compareAndSet(FondedPos.rightNode, Neighbor, false, false);
                return true;
            }
        }
    }

    @Override
    public boolean contains(T value) {
        Node<T> it = head.next.getReference();
        boolean[] marked = {false};
        while (it != tail && it.value.compareTo(value) < 0) {
            it.next.get(marked);
            it = it.next.getReference();
        }
        return value == it.value && !marked[0];
    }

    @Override
    public boolean isEmpty() {
        while (head.next.getReference() != tail) {
            Node<T> it = head.next.getReference();
            if (!it.next.isMarked()) {
                return false;
            }
            head.next.compareAndSet(it, it.next.getReference(), false, false);
        }
        return true;
    }
}