using System;
using System.Diagnostics; /*Debug.Assert*/
using System.IO; /**/
using System.Net; /**/
using System.Net.Sockets; /**/
using System.Text; /**/
using System.Threading; /**/

namespace SimpleWebServer
{

    


    public class WebServer
    {

        private TcpListener mylistener;
        private IPAddress localaddr;
        private string ipstring = "127.0.0.1";
        private int PORT = 5050;
        private String sMyWebServerRoot = "/Users/levio/dsa/webserver";

        public WebServer()
        {
            try {
                /* TcpListener(int port) has been deprecated, please use TcpListener(IPAddress localaddr, int port) */
                localaddr = IPAddress.Parse(ipstring);
                Console.WriteLine(localaddr);
                mylistener = new TcpListener(localaddr, PORT);
                mylistener.Start();
                Console.WriteLine("web server started ... press ^C to stop.");

                /* Start the thread which calls the method StartListen */
                Thread th = new Thread(new ThreadStart(StartListen));
                th.Start();

            } catch (Exception e) {
                Console.WriteLine("Error: " + e.ToString()); // doesn't work with a comma -- why? Want to use the thing as an argument to teh format string but there is no fmt string
            }
        }

        /* accept a directory, check a file for default file names. Iterate over the list.
         * If one is a valid file, take it and return the concatenation of the local dir 
         * with the file. */
        public string GetDefaultFileName(string localdir)
        {
            string defaultfile = "defaults.txt";
            string sline = ""; /* Needs to be defined, otherwise compile time error for using it outside the try/catch */
            try
            {
                Debug.Assert(File.Exists(defaultfile));
                StreamReader reader = new StreamReader(defaultfile);
                while ((sline = reader?.ReadLine()) != null)
                {
                    if (File.Exists(sline))
                        break;
                }
                Debug.Assert(File.Exists(localdir+sline));
            } 
            catch (Exception e) 
            {
                Console.WriteLine("An Exception Occurred resolving the default file:" + e.ToString());
                throw;
            }
            return localdir + sline;
        }

        /* Read from local directory mapping file to resolve local directory
         * from virtual directory. */
        /* Expected layout of Vdirs file "data/Vdirs.dat" :
         * virtualdir;realdir\n ...
         */
        public String GetLocalPath(String localRoot, String sRequestDir)
        {
            StreamReader sr;
            String sLine = "";
            String sVirtualDir = "";
            String sRealDir = "";
            int iStartPos = 0;
            sRequestDir.Trim();
            localRoot = localRoot.ToLower();
            sRequestDir = sRequestDir.ToLower();
            try {
                sr = new StreamReader("data/Vdirs.dat");
                while ((sLine = sr.ReadLine()) != null) {
                    sLine.Trim();
                    if (sLine.Length > 0)
                    {
                        iStartPos = sLine.IndexOf(";");
                        sLine = sLine.ToLower();
                        sVirtualDir = sLine.Substring(0, iStartPos);
                        sRealDir = sLine.Substring(iStartPos + 1);
                        if (sVirtualDir == sRequestDir)
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("An Exception occurred while getting local path: " + e.ToString()); 
            }
            if (sVirtualDir == sRealDir)
                return sRealDir;
            else
                return "";
        }

        /* Resolve media type based on the user supplied file extension. */
        /* What is MIME type? Media type of a file. */
        /* Mime resolution file "data/mime.dat" : (maps mime extensions to mime types)
         * sMimeExt;sMimeType ...
         */
        public String GetMimeType(string sRequestedFile)
        {
            StreamReader sr;
            String sLine = "";
            String sFileExt = "";
            String sMimeExt = "";
            String sMimeType = "";

            sRequestedFile = sRequestedFile.ToLower();
            int iStartPos = sRequestedFile.IndexOf(".");
            sFileExt = sRequestedFile.Substring(iStartPos); // substring including "."
            try
            {
                sr = new StreamReader(sRequestedFile);
                while ((sLine = sr.ReadLine()) != null) {
                    sLine.Trim();
                    if (sLine.Length > 0) {
                        iStartPos = sLine.IndexOf(";");
                        sLine = sLine.ToLower();
                        sMimeExt = sLine.Substring(0, iStartPos);
                        sMimeType = sLine.Substring(iStartPos + 1);
                        if (sMimeExt == sFileExt)
                            break;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("An exception occured resolving the MIME type: " + e.ToString());
            }
            return (sMimeExt == sFileExt) ? sMimeType : "";
        }

        /* Build and send headers to the browser (client). */
        public void SendHeader(string sHTTPVersion, string sMIMEHeader, int iTotBytes, string sStatusCode, ref Socket mysocket)
        {
            String sBuffer = "";
            /* Set default MIME type to plain text. */
            if (sMIMEHeader.Length == 0)
                sMIMEHeader = "text/html";

            sBuffer = sBuffer + sHTTPVersion + sStatusCode + "\r\n";
            sBuffer = sBuffer + "Server: cx1193719-b\r\n";
            sBuffer = sBuffer + "Content-Type: " + sMIMEHeader + "\r\n";
            sBuffer = sBuffer + "Accept-Ranges: bytes\r\n";
            // you can concatenate an integer into a string?
            sBuffer = sBuffer + "Content-Length: " + iTotBytes + "\r\n\r\n";

            Byte[] bSendData = Encoding.UTF8.GetBytes(sBuffer);
            SendToBrowser(bSendData, ref mysocket);
            Console.WriteLine($"Total Bytes: {iTotBytes}");
        }

        /* Overloaded. */
        public void SendToBrowser(string sData, ref Socket mySocket)
        {
            SendToBrowser(Encoding.UTF8.GetBytes(sData), ref mySocket);
        }

        public void SendToBrowser(byte[] bSendData, ref Socket mySocket)
        {
            int numBytes = 0;
            try
            {
                Debug.Assert(mySocket.Connected);
                if (mySocket.Connected)
                {
                    if ((numBytes = mySocket.Send(bSendData)) == -1)
                        Console.WriteLine("Socket failed to send Packet.");
                    else
                        Console.WriteLine("Number of bytes sent {0}", numBytes);
                }
                else {
                    Console.WriteLine("Connection Dropped...");
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        public void StartListen()
        {
            int iStartPos;
            int TotBytes;
            String sRequest;
            String sRequestedFile;
            String sDirName;
            String sLocalDir;  // physical address
            String sErrorMessage;
            String sMimeType;
            String sPhysicalFilePath;
            String sResponse;

            while (true) {
                Socket mysocket = mylistener.AcceptSocket();
                Console.WriteLine("Socket Type {0}", mysocket.SocketType);
                if (mysocket.Connected) {
                    Console.WriteLine("Hooray!");
                    /* Create a data buffer and read data from the client. */
                    Byte[] bReceive = new Byte[1024];
                    int i = mysocket.Receive(bReceive,bReceive.Length, 0);
                    /* Convert buffer to string look for HTTP request info with string
                     * manipulations. */
                    // String bufString = bReceive.ToString();
                    String sBuffer = Encoding.UTF8.GetString(bReceive);
                    /* Look for a GET request and look for a URL. */
                    if (sBuffer.Substring(0, 3) != "GET")
                    {
                        Console.WriteLine("Only GET supported...");
                        mysocket.Close();
                        return;
                    }
                    iStartPos = sBuffer.IndexOf("HTTP", 1);
                    String sHTTPVersion = sBuffer.Substring(iStartPos, 8);
                    /* Process Request */
                    sRequest = sBuffer.Substring(iStartPos-1);
                    sRequest.Replace('\\', '/');

                    /* We want to return a directory if the file name contains no '.' */
                    if ((sRequest.IndexOf('.') < 1) && (!sRequest.EndsWith('/')))
                        sRequest = sRequest + '/';

                    /* Extract file name from end of request. */
                    iStartPos = sRequest.LastIndexOf('/') + 1;
                    sRequestedFile = sBuffer.Substring(iStartPos);
                    sDirName = sRequest.Substring(sRequest.IndexOf('/'), sRequest.LastIndexOf('/') -3);


                    /* Get Physical Address */
                    if (sDirName == "/")
                    { // root
                        sLocalDir = sMyWebServerRoot;

                    } 
                    else
                    {
                        sLocalDir = GetLocalPath(sMyWebServerRoot, sDirName);
                    }
                    Console.WriteLine("Directory Requested : {0}", sLocalDir );
                    if (sLocalDir.Length == 0)
                    {
                        sErrorMessage = "\n<H2>Error! Please check </H2><Br>";
                        SendHeader(sHTTPVersion, "", sErrorMessage.Length, "404 Not Found", ref mysocket);
                        /* What is the SendHeader function? */
                        SendToBrowser(sErrorMessage, ref mysocket);
                        mysocket.Close();
                        continue;
                    }

                    /* Check length of file name, if 0 get file from default file list. */
                    if (sRequestedFile.Length == 0)
                        sRequestedFile = GetDefaultFileName(sLocalDir);
                    /* TODO Error handling */
                    sMimeType = GetMimeType(sRequestedFile);
                    sPhysicalFilePath = sLocalDir + sRequestedFile;

                    Console.WriteLine("File Requested: {0}", sPhysicalFilePath);

                    if (!File.Exists(sPhysicalFilePath))
                    {
                        /* TODO Error handling */
                    }
                    else {
                        int iTotBytes = 0;
                        FileStream fs = new FileStream(sPhysicalFilePath, FileMode.Open, FileAccess.Read, FileShare.Read);
                        BinaryReader reader = new BinaryReader(fs);
                        int read;
                        Byte[] bytes = new Byte[fs.Length]; // what is this doing? File stream has length property?
                        sResponse = "";

                        while ((read = reader.Read(bytes, 0, bytes.Length)) != 0) {
                            sResponse = sResponse + Encoding.UTF8.GetString(bytes, 0, read);
                            TotBytes = iTotBytes + read;
                        }
                        reader.Close();
                        fs.Close();
                        SendHeader(sHTTPVersion, sMimeType, iTotBytes, " 200 OK", ref mysocket);
                        SendToBrowser(bytes, ref mysocket);
                        // mysocket.Send(bytes, bytes.Length, 0);

                    }
                    mysocket.Close();

                }
                mysocket.Close();
            }
            
        }

    }
    
    public class Program
    {
        public static void Main(string[] args)
        {

            WebServer myws = new WebServer();
        }
    }
}