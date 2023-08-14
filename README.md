# Find-and-Replace-Text-in-File
Implemented a program that reads command line arguments and performs find/replace on files.

<h2>Description</h2>
This program would perform the find and replace function on the specified input file. Unless the same file is entered as both the input and output files, the input file will never be modified. The results of the find/replace would be printed in the specified output file (Note: output file would be overwritten if the file is not empty). To use this program, users must enter a command line using the required flags (see below) to indicate the search replacement texts. The command should also state the input file and output file. Despite that, there are optional flags that individuals can use to modify the replacement process. For example, users can use the l flag to indicate which lines the find/replace function should be performed and the w flag to perform prefix/suffix replacement (replace all words with the specified prefix/suffix). 
<br><br>

<b>Flags:</b>
<ul>
    <li>-s [string: search text] (required) (ex. -s world)</li>
    <li>-r [string: replacement test] (required) (ex. -r Hello)</li>
    <li>-l [int: start, int: end] (optional) (ex. -l 1,2 => only perform the find/replace function between lines 1 and 2 inclusive)</li>
    <li>-w (optional) (no argument, but search text must be entered as *world for suffix find/replace or world* for prefix find/replace)</li>
</ul>

<b>Note/Warning:</b>
<ul>
  <li>The input order doesn't matter for the flags listed above.</li>
  <li>The input and output files must be entered as the last two command line arguments, with input coming before output</li>
  <li>Due to certain reasons, the prefix/suffix find/replace function doesn't support any words that start with a space or punctuation</li>
</ul>

<b>Error Codes - </b>When the input command line is invalid, the program returns an error code in the following order of precedence:
<ol>
  <li>MISSING_ARGUMENT (code 1 - when the input command line is less than seven arguments)</li>
  <li>DUPLICATE_ARGUMENT (code 8 - when a flag is entered twice)</li> 
  <li>INPUT_FILE_MISSING (code 2 - when the indicated input file couldn't be opened or doesn't exist)</li>
  <li>OUTPUT_FILE_UNWRITABLE (code 3 - when the indicated output file is not writable)</li>
  <li>S_ARGUMENT_MISSING (code 4 - when the s flag or s flag argument is missing)</li>
  <li>R_ARGUMENT_MISSING (code 5 - when the r flag or r flag argument is missing)</li>
  <li>L_ARGUMENT_INVALID (code 6 - when the l flag argument is invalid)</li>
  <li>WILDCARD_INVALID (code 7 - when the w flag is entered but the search text indicated by the s flag is invalid)</li>
</ol>
<h2>Get Started</h2>
<h3>Dependencies</h3>
<ul>
    <li>Windows or Linux operating system</li>
    <li><a href="https://www.scaler.com/topics/c/c-compiler-for-windows/">GNU Compiler Collection (GCC)</a></li>
    <li>Text Editor or IDEs</li>
    <h4>Recommend:</h4>
    <ul>
        <li>Notepad (Worst Case)</li>
        <li><a href="https://www.codeblocks.org/downloads/binaries/">Code::Blocks</a> (Please download the one with GCC compiler)</li>
        <li><a href="https://www.eclipse.org/downloads/">Eclipse</a></li>
        <li><a href="https://code.visualstudio.com/Download">Visual Studio Code</a></li>
    </ul>
</ul>

<h3>Installing</h3>
<strong>Reference: </strong><a href="https://docs.github.com/en/get-started/quickstart/fork-a-repo">https://docs.github.com/en/get-started/quickstart/fork-a-repo</a>
<ol>
    <li>Fork this repository by clicking the Fork button located on the top of the page</li>
    <li>Navigate to "Your Repositories" on GitHub and open the forked repo</li>
    <li>In the terminal, git clone the repo using the URL or SSH key</li>
</ol>

<h3>Executing Program</h3>
<h4>Run with makeFile: </h4>
In the terminal, type <code>make</code> and <code>make test</code> to run the test cases written in unit_tests.c. Individuals may use the existing code as an example and write their test cases.
<br><br>
<b>Note: </b> If including your own test cases, you must include your input file in tests.in.orig and expected output file in tests.out.exp.

<h4>Run with GCC: </h4>
In the terminal, type <code>gcc [source file]</code>. By default, this will produce an executable file named a.out (Linux) or a.exe (Windows). If you want the executable file to have a custom name, type <code>gcc -o [name of executable] [source file]</code>. After compiling the program, run the executable file by entering <code>./[executable file] [command line arguments for running the program]</code>.

<h2>Author</h2>
<strong>Zhen Wei Liao</strong> - <a href="https://www.linkedin.com/in/zhenwei-liao-148baa273">@Zhen</a><br>
Built the main program and parts of FindReplace.h/unit_tests.c

<h3>Contributor:</h3>
<strong>Professor Kevin McDonnell</strong> - <a href="https://www.cs.stonybrook.edu/people/faculty/KevinMcDonnell">@KevinMcDonnell</a><br>
Created the makeFile, unit_tests.h, and parts of unit_tests.c/FindReplace.h

<h2>License</h2>
This project is under the MIT License. <br>Please check the LICENSE.md file for more information.
