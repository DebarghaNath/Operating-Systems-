    <style>
        body {
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
        }
        .container {
            width: 80%;
            margin: auto;
            overflow: hidden;
        }
        header {
            background: #333;
            color: #fff;
            padding-top: 30px;
            min-height: 70px;
            border-bottom: #ccc 3px solid;
            text-align: center;
        }
        header h1 {
            margin: 0;
            font-size: 24px;
        }
        main {
            padding: 20px;
            background: #fff;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h2 {
            color: #333;
        }
        pre {
            background: #f8f8f8;
            border-left: 3px solid #ccc;
            padding: 10px;
            overflow-x: auto;
        }
        code {
            background: #f8f8f8;
            padding: 2px 4px;
            border-radius: 4px;
        }
        ul {
            margin: 0;
            padding: 0;
            list-style: none;
        }
        li {
            margin: 5px 0;
        }
        footer {
            text-align: center;
            padding: 20px;
            background: #333;
            color: #fff;
            position: absolute;
            bottom: 0;
            width: 100%;
        }
    </style>
</head>
<body>
    <header>
        <div class="container">
            <h1>Operating Systems Repository</h1>
        </div>
    </header>
    <main class="container">
        <h2>Welcome to the Operating Systems Repository!</h2>
        <p>This collection of programs is designed to help you explore fundamental operating system concepts through practical examples. Each program demonstrates key techniques and functionalities related to system-level programming in C.</p>

        <h2>Contents</h2>

        <h3>1. Shared Memory</h3>
        <p>This section contains examples of creating and managing shared memory between two independent processes. Shared memory is a powerful method for inter-process communication (IPC) that allows multiple processes to access the same memory space, facilitating data exchange and synchronization.</p>
        
        <h4>Features:</h4>
        <ul>
            <li>Creating shared memory segments.</li>
            <li>Attaching and detaching shared memory to/from processes.</li>
            <li>Synchronization mechanisms for coordinating access to shared memory.</li>
        </ul>

        <h4>Usage:</h4>
        <pre><code>1. Compile the Program:
   gcc -o shared_memory shared_memory.c

2. Run the Program:
   Open two terminal windows and run the compiled program in each terminal to observe the shared memory in action.</code></pre>

        <h3>2. Memory Mapped Files</h3>
        <p>This section demonstrates the use of memory-mapped files through the <code>mmap</code> system call. Memory-mapped files provide a mechanism for mapping files or devices into memory, allowing processes to read from and write to files as if they were memory locations.</p>
        
        <h4>Features:</h4>
        <ul>
            <li>Mapping files into the address space of a process.</li>
            <li>Reading and writing data through the mapped memory.</li>
            <li>Handling file changes and synchronization.</li>
        </ul>

        <h4>Usage:</h4>
        <pre><code>1. Compile the Program:
   gcc -o memory_mapped_files memory_mapped_files.c

2. Run the Program:
   Execute the compiled program to see how memory-mapped files work and interact with file data.</code></pre>

        <h2>Getting Started</h2>
        <pre><code>1. Clone the Repository:
   git clone https://github.com/yourusername/operating-systems.git

2. Navigate to the Directory:
   cd operating-systems

3. Compile and Run Programs:
   Follow the usage instructions for each program provided in their respective sections.</code></pre>

        <h2>Contributing</h2>
        <p>Feel free to contribute to this repository by submitting issues, feature requests, or pull requests. Your contributions are greatly appreciated!</p>

        <h2>License</h2>
        <p>This project is licensed under the MIT License. See the <code>LICENSE</code> file for details.</p>
    </main>
    <footer>
        <p>&copy; 2024 Operating Systems Repository</p>
    </footer>
</body>
</html>
