from threading import Lock


class DiningPhilosophers:

    def __init__(self):
        self.forks = [Lock() for _ in range(5)] 

    # call the functions directly to execute, for example, eat()
    def wantsToEat(self,
                   philosopher: int,
                   pickLeftFork: 'Callable[[], None]',
                   pickRightFork: 'Callable[[], None]',
                   eat: 'Callable[[], None]',
                   putLeftFork: 'Callable[[], None]',
                   putRightFork: 'Callable[[], None]') -> None:

        if philosopher == 0:
            with self.forks[philosopher]:
                pickLeftFork()
                with self.forks[philosopher - 1]:
                    pickRightFork()
                    eat()
                    putRightFork()
                putLeftFork()
        else:
            with self.forks[philosopher - 1]:
                pickRightFork()
                with self.forks[philosopher]:
                    pickLeftFork()
                    eat()
                    putLeftFork()
                putRightFork()
