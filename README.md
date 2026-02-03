<p align="center">
  <img src="logo.png" alt="" width="280"/>
</p>

<h1 align="center"><strong>refn - Reads specific lines of text documents</strong></h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?logo=cplusplus" alt="">
  <img src="https://img.shields.io/codefactor/grade/github/linuxproativo/refn?logo=codefactor" alt="">
  <img src="https://img.shields.io/badge/License-GPLv3-red.svg" alt="">
  <img src="https://img.shields.io/badge/Binary_Size-%3C_1.5MB-green" alt="">
  <img src="https://img.shields.io/github/repo-size/linuxproativo/refn?color=brightgreen" alt="">
  <img src="https://img.shields.io/github/last-commit/linuxproativo/refn?color=green" alt="">
  <img src="https://img.shields.io/github/v/release/linuxproativo/refn?color=orange" alt="">
</p>

The **refn** utility is a high-performance CLI tool designed to extract specific 
lines from text documents with surgical precision. While standard Unix tools like 
`head` and `tail` are efficient for file extremities, they often require complex
piping or cryptic syntax to access specific middle ranges or inverted offsets.

The core philosophy of **refn** is to provide a unified, 
intuitive interface that handles edge cases automatically 
***such as requests for ranges that exceed the file length***
without forcing the user to resort to fragile shell "one-liners".

## 📊 The Competition: Standard Shell vs. refn
Below is a comparison between refn and the most optimized way to achieve the
same result using standard POSIX tools (`sed`, `awk`, `tail`, `head`).

<table>
  <thead>
    <tr>
      <th>Task Description</th>
      <th>Traditional Optimized Shell Command</th>
      <th><b>refn</b> Command</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Extract exactly line 10</td>
      <td><code>sed -n '10p;10q' file.txt</code></td>
      <td><code>refn -d10 file.txt</code></td>
    </tr>
    <tr>
      <td>Extract line 10 (AWK)</td>
      <td><code>awk 'NR==10 {print; exit}' file.txt</code></td>
      <td><code>refn -d10 file.txt</code></td>
    </tr>
    <tr>
      <td>Extract range 10 to 20</td>
      <td><code>sed -n '10,20p;20q' file.txt</code></td>
      <td><code>refn -d10-20 file.txt</code></td>
    </tr>
    <tr>
      <td>Extract range 10 to 20 (AWK)</td>
      <td><code>awk 'NR>=10 && NR<=20' file.txt</code></td>
      <td><code>refn -d10-20 file.txt</code></td>
    </tr>
    <tr>
      <td>Extract range (Tail/Head)</td>
      <td><code>tail -n +10 file.txt | head -11</code></td>
      <td><code>refn -d10-20 file.txt</code></td>
    </tr>
    <tr>
      <td>First 10 lines</td>
      <td><code>head file.txt</code></td>
      <td><code>refn file.txt</code></td>
    </tr>
    <tr>
      <td>Last 10 lines</td>
      <td><code>tail file.txt</code></td>
      <td><code>refn -l file.txt</code></td>
    </tr>
    <tr>
      <td>10th line from the end</td>
      <td><code>tail file.txt | head -1</code></td>
      <td><code>refn -i10 file.txt</code></td>
    </tr>
    <tr>
      <td>5th line from the end (AWK)</td>
      <td><code>awk '{a[NR]=$0} END {print a[NR-4]}' file</code></td>
      <td><code>refn -i5 file.txt</code></td>
    </tr>
    <tr>
      <td>Range 50-60 (Simple output)</td>
      <td><code>sed -n '50,60p;60q' file.txt</code></td>
      <td><code>refn -sd50-60 file.txt</code></td>
    </tr>
    <tr>
      <td>Line 10 of multiple files</td>
      <td><code>head -q -n 10 f1 f2 | tail -n 1</code></td>
      <td><code>refn -d10 f1 f2</code></td>
    </tr>
    <tr>
      <td>Line 100 with line number</td>
      <td><code>grep -n "" file | sed -n '100p;100q'</code></td>
      <td><code>refn -d100 file.txt</code></td>
    </tr>
    <tr>
      <td>Extract 20th line (Pipe mess)</td>
      <td><code>cat -n f | grep -w 20 | cut -f2-</code></td>
      <td><code>refn -d20 -s file.txt</code></td>
    </tr>
    <tr>
      <td>First line of a file</td>
      <td><code>head -1 file.txt</code></td>
      <td><code>refn -f1 file.txt</code></td>
    </tr>
    <tr>
      <td>Last line of a file</td>
      <td><code>tail -1 file.txt</code></td>
      <td><code>refn -l1 file.txt</code></td>
    </tr>
    <tr>
      <td>Middle section (500-505)</td>
      <td><code>sed -n '500,505p;505q' file.txt</code></td>
      <td><code>refn -d500-505 file.txt</code></td>
    </tr>
    <tr>
      <td>100th line from end (Clean)</td>
      <td><code>tail -100 file | head -1</code></td>
      <td><code>refn -si100 file.txt</code></td>
    </tr>
    <tr>
      <td>First 5 lines (AWK)</td>
      <td><code>awk 'NR <= 5' file.txt</code></td>
      <td><code>refn -f5 file.txt</code></td>
    </tr>
    <tr>
      <td>Line 10 with colors (Grep)</td>
      <td><code>grep --color=always -n "^" f | sed...</code></td>
      <td><code>refn -cd10 file.txt</code></td>
    </tr>
    <tr>
      <td>Range 5-10 from multiple files</td>
      <td><code>for f in *; do sed -n '5,10p' $f; done</code></td>
      <td><code>refn -d5-10 *</code></td>
    </tr>
  </tbody>
</table>

### 💡 Key Takeaways

1. **Consistency:** Standard tools require switching between `sed`, `awk`, `head`,
and `tail` depending on the task. refn uses the same flags for everything.

2. **Safety First:** When using `tail -n +X | head -n Y`, if the file is smaller
than X, the behavior can be unpredictable across different shell environments.
refn performs boundary checks to ensure valid output.

3. **Performance Optimization:** In the Shell commands above, we used `;q` (quit)
in `sed` and `exit` in `awk`. Without these, they would read the entire file even
if you only wanted the first line. refn does this automatically by design.

4. **Process Overhead:** Every `|` (pipe) in the table above creates a new process in
your OS. refn runs as a single, highly optimized C++ binary, saving CPU cycles and memory.

### 🚀 Recommendation

For automation and scripts, use the `-s` (Simple) flag to get raw text. For terminal
inspection, use the `-c` (Color) flag for better readability.

## 🛠️ Installation and Build

The project utilizes **CMake** and focuses on size optimization (`-Os`)
and high performance.

```bash
mkdir build && cd build
cmake ..
make
sudo cp refn /usr/bin
```

> Note: The build process generates a standalone, portable binary with no external dependencies.

## ⚙️ Parameters

The **refn** it has a series of parameters. See the follow:
```bash
Usage: refn [OPTIONS] [FILE]...

Operations:
  -f, --first=[N]    Display the first N lines (default: 10).
  -l, --last=[N]     Display the last N lines (default: 10).
  -d, --delimit=R    Display a specific range R (e.g., 10-20 or 50).
  -i, --invert=R     Display a specific range R in inverted order.

Options:
  -c, --color        Enable syntax-like coloring for line numbers.
  -s, --simple       Plain text output (no line numbers or headers).
  -h, --help         Display this help information and exit.
```

## 🚀 Comparation and Use

The use of **refn** is very simple. Let's start with the basic commands.
How was said, this command has the functions of *head* and *tail*, but with
some improvements, as can be seen folow:
```bash
$ head -5 LICENSE 
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
```

```bash
$ refn -f5 LICENSE 
/home/mauricio/GIT/refine/LICENSE:
         1:                     GNU GENERAL PUBLIC LICENSE
         2:                        Version 3, 29 June 2007
         3: 
         4:  Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
         5:  Everyone is permitted to copy and distribute verbatim copies
```

```bash
$ refn -sf5 LICENSE 
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
```

Now, compared to the *tail*. As you can see, the last line does not enter
the equation if it is empty, just like the *tail*:
```bash
$ tail -5 LICENSE 
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
<https://www.gnu.org/licenses/why-not-lgpl.html>.
```

```bash
$ refn -l5 LICENSE 
/home/mauricio/GIT/refine/LICENSE:
       670: into proprietary programs.  If your program is a subroutine library, you
       671: may consider it more useful to permit linking proprietary applications with
       672: the library.  If this is what you want to do, use the GNU Lesser General
       673: Public License instead of this License.  But first, please read
       674: <https://www.gnu.org/licenses/why-not-lgpl.html>.
```

```bash
$ refn -sl5 LICENSE 
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
<https://www.gnu.org/licenses/why-not-lgpl.html>.
```

Now comes the part that makes sense of the command existance. Let's compare
the output of specific numbers of lines, which, in the example, will be listed
lines in 26 to 30, as can be seen folow:
```bash
$ sed -n '26,30p;30q' file.txt
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.

  To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
```

```bash
$ refn -d26-30 LICENSE 
/home/mauricio/GIT/refine/LICENSE:
        26: want it, that you can change the software or use pieces of it in new
        27: free programs, and that you know you can do these things.
        28: 
        29:   To protect your rights, we need to prevent others from denying you
        30: these rights or asking you to surrender the rights.  Therefore, you have
```

```bash
$ refn -sd26-30 LICENSE 
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.

  To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
```

Now, to close with golden key, let's analyze the code used to read only a specific
line, as in the following example:
```bash
$ awk 'NR==10 {print; exit}' LICENSE
  The GNU General Public License is a free, copyleft license for
```

```bash
$ refn -d10 LICENSE 
/home/mauricio/GIT/refine/LICENSE:
        10:   The GNU General Public License is a free, copyleft license for
```

```bash
$ refn -sd10 LICENSE 
  The GNU General Public License is a free, copyleft license for
```

You want the tenth last line? Here it is:
```bash
$ tail /etc/slackpkg/mirrors | head -1
# https://mirror.slackbuilds.org/slackware/slackware64-current/
```

```bash
$ refn -i10 /etc/slackpkg/mirrors 
/etc/slackpkg/mirrors:
       353: # https://mirror.slackbuilds.org/slackware/slackware64-current/
```

```bash
$ refn -si10 /etc/slackpkg/mirrors 
# https://mirror.slackbuilds.org/slackware/slackware64-current/
```

The result dispenses with comments. So, to conclude, here is a command alternative
to facilitate the search for specific lines in files.

## ⚖️ GNU General Public License

This repository has scripts that were created to be free software.<br/>
Therefore, they can be distributed and / or modified within the terms of the *GNU General Public License*.

>[General Public License](https://pt.wikipedia.org/wiki/GNU_General_Public_License)
>
>Free Software Foundation (FSF) Inc. 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

## 🤝 Contributing

Contributions are what make the open-source community such an amazing place
to learn, inspire, and create. Any contributions you make are
**greatly appreciated**.

### 📌 How to Contribute

1. **Fork** the Project.
2. **Create your Feature Branch** (`git checkout -b feature/AmazingFeature`).
3. **Commit your Changes** (`git commit -m 'Add some AmazingFeature'`).
4. **Push to the Branch** (`git push origin feature/AmazingFeature`).
5. **Open a Pull Request**.

### 📌 Coding Guidelines
To maintain the high performance and portability of **refn**,
please adhere to the following:
* **C++ Standard:** Use **C++23** features where applicable, but avoid heavy libraries
that might bloat the binary size.
* **Performance First:** When modifying I/O logic, always test the impact
on large files (multi-GB). The use of buffered reading is mandatory.
* **Portability:** Avoid OS-specific dependencies (Linux/Windows) unless absolutely
necessary, ensuring the tool remains portable.

### 📌 Roadmap & Ideas
We are currently looking for help with:
* Optimization of line-counting algorithms.
* Refinement of the argument parser.
* Support for reading compressed files (e.g., `.gz`) without prior extraction.

## 📩 Contact

Autor: Mauricio Ferrari (LinuxProativo)  
E-Mail: *m10ferrari1200@gmail.com - contatolinuxdicaspro@gmail.com*
