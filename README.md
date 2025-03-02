# 42 Philosopher

███╗░░██╗░█████╗░░█████╗░██████╗░██╗░░░░░███████╗░██████╗
████╗░██║██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔════╝██╔════╝
██╔██╗██║██║░░██║██║░░██║██║░░██║██║░░░░░█████╗░░╚█████╗░
██║╚████║██║░░██║██║░░██║██║░░██║██║░░░░░██╔══╝░░░╚═══██╗
██║░╚███║╚█████╔╝╚█████╔╝██████╔╝███████╗███████╗██████╔╝
╚═╝░░╚══╝░╚════╝░░╚════╝░╚═════╝░╚══════╝╚══════╝╚═════╝░

## 📖 What is `Philosophers` 🍝?
<details>
<summary><b>A dining philosophers problem implementation</b></summary><br>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Philosophers is a project associated with the curriculum of École 42, a coding school known for its project-based learning approach.
The Philosophers project uses the C programming language to solve the classic "dining philosophers problem" - a synchronization and concurrency challenge.
The goal of this project is to teach students about threads, mutexes, and the complexities of concurrent programming while avoiding deadlocks and race conditions.
</p>
</details>

## ✅ Status
<details>
<summary><b>Project completion status</b></summary><br>
<p align="center">
Completed on : 2025-02-05 <br> 100/100
</p>
</details>

## 🚀 Usage
To clone and compile:
```shell
git clone https://github.com/Patthh/42_Philosophers.git
cd 42_philosophers && make
```

To run the program:
```shell
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat(Optional)]
```

Example:
```shell
./philo 5 800 200 200 7
```

## ✨ Features
- 🧠 Each philosopher is represented by a thread
- 🍴 Forks are represented by mutexes
- ⏱️ Precise timing system using gettimeofday
- 💀 Death detection without busy waiting
- 🏁 Proper program termination without leaks
- 🔄 Robust synchronization to prevent data races
- 📊 Status messages for all philosopher actions:
  - 🤔 Thinking
  - 🍴 Taking a fork
  - 🍝 Eating
  - 😴 Sleeping
  - ☠️ Dying
- 🔍 Error handling for invalid inputs
- 📏 Clean code organization following 42's Norm

> [!WARNING]
> Incorrect implementation may lead to deadlocks or philosophers starving to death

## 🛠️ Implementation Details
<details>
<summary><b>Program Structure</b></summary><br>
<p>The Philosophers program consists of these main components:</p>

| Component | Description |
|---------|-------------|
| 🧵 Threads | Each philosopher runs as an independent thread |
| 🔒 Mutexes | Protect shared resources (forks) from race conditions |
| ⏲️ Timers | Track time_to_die, time_to_eat, time_to_sleep |
| 🔄 Routines | Define philosopher behaviors in concurrent execution |
| 📝 Monitoring | Check for deaths or completion conditions |

</details>

> [!NOTE]
> "I think, therefore I am... hungry"

<div align="center">
  <img src="https://media1.tenor.com/m/aNzzrsihnysAAAAC/albertcamus.gif" width="400" alt="Dining Philosophers">
  <br>
  <i>A journey into concurrent programming</i><br><br><br>
</div>

---
<div align="center">
  <p>Made with 💭💭💭s</p>
</div>
