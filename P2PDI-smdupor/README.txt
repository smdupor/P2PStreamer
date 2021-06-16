Compatibility: This program is fully compatible and tested with Ubuntu 14 through 21. Requires linux networking headers.

QUICK-START GUIDE:

1. scp the tarball to the systems you want to test on.

2. On each computer, Untar the file, cd into the directory, and run make. If you are running all processes on one computer,
    the build only needs to be run once:

    tar -xf P2PDI-smdupor.tar.gz
    cd P2PDI-smdupor
    make

3. (For NCSU VCL): If you have not added IPTABLES rules to permit traffic used by this app, run the iptables
     configuration (requires sudo):

    ./configure_iptables.sh

4. Start components. It is highly suggested that you start these from 3 separate terminal windows, otherwise it will be
    difficult to discern which component is printing which message at any given time. In this example, we will consider
    a scenario where the Registration Server is running on a host with IP 10.0.0.3. Note that you can run all 3 components
    on the same host using the localhost loopback adapter, but the client will notify you that referring to the Registration
    Server as localhost or 127.0.0.1 will cause it's IP to be detected as localhost ( a problem if the other client process is
    located on a remote network host).

    If you wish to continue with localhost, simply respond with ('n') when asked if you want to pick a different hostname.

*********Terminal 1 (10.0.0.3): Start the registration server:

    ./RegServ

*********Terminal 2: Start client (a) (substituting 10.0.0.3 with your Registration Server's IP):

    ./Client a 10.0.0.3

*********Terminal 3: Start client (b) (substituting 10.0.0.3 with your Registration Server's IP):

    ./Client b 10.0.0.3

5. EXITING: Client (b) will automatically leave and exit the program.

    ** For client A and the registration server:

    SHUTDOWN NOTE: Because the host that does not leave will continue requesting to find new peers in perpetuity, after
    ten seconds of system inactivity, it will pause (the download thread, not the server and keepalive threads) and ask
    if you want to continue or exit.

    Answering n (NO) to continuing will allow this client to leave the system, and shut
    down the Registration Server Gracefully. This way, the OS will properly close/free all resources (rather than using
    ctrl-c to sigterm the remaining processes).

6. CONFIRMING SUCCESS:
    Downloaded files are stored (for all clients) in the ./Downloads directory. On client A, you may compare the
    downloaded file to the original using diff:

    diff ./Downloads/Downloads_A/rfc9026.txt ./Downloads/Downloads_B/rfc9026.txt

    (or)

    diff ./Downloads/Downloads_A/rfc9035.txt ./Downloads/Downloads_B/rfc9035.txt

    Depending on which file was chosen to download. The program uses a randomization algorithm for choosing which file
    to request at any given moment (to spread load across files and hosts), so either of these files may be the one
    picked.


APPENDIX: Directory Structure:
./conf -- configuration files that tell the clients which client they are to simulate, their files, etc

./include -- .h header files for all c++ classes
./main -- int main() files for the Client and the Registration Server executables
./src -- .cpp source files for all c++ classes

./Downloads/Downloads_A -- RFC Files for client A. Starts empty.
./Downloads/Downloads_B -- RFC Files for client B. Starts with 2 rfc text files.

./bin -- holds the compiled binaries. Please run the program using the included symlinks in the root directory.
./obj  -- Temporary storage of object files created at compile time
./logs -- CSV files with download time datapoints will be written here.

Appendix: Program Structure:
***** Object Classes:
FileEntry           -- data structure element for an RFC index item, member of the rfc files linked list
PeerNode            --  data structure element for a peer index item, member of the peers linked list
NetworkCommunicator -- Superclass holding shared functionality between both Registration Server and P2PClients
RegistrationServer  -- Subclass object for all registration server functionality
P2PClient           -- Subclass object for all Filesharing nodes (File server, Registration Client,  File Client)