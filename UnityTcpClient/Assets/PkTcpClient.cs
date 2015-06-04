using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System;

public class PkTcpClient : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        try
        {
            // Create a TcpClient.
            // Note, for this client to work you need to have a TcpServer 
            // connected to the same address as specified by the server, port
            // combination.
            int port = 1337;
            TcpClient client = new TcpClient("127.0.0.1", port);

            // Translate the passed message into ASCII and store it as a Byte array.
            Byte[] data = System.Text.Encoding.ASCII.GetBytes("test from unity");

            // Get a client stream for reading and writing.
            //  Stream stream = client.GetStream();

            NetworkStream stream = client.GetStream();

            // Send the message to the connected TcpServer. 
            stream.Write(data, 0, data.Length);

            Debug.Log("Sent : test from unity");

            //// Receive the TcpServer.response.
            //
            //// Buffer to store the response bytes.
            //data = new Byte[256];
            //
            //// String to store the response ASCII representation.
            //String responseData = String.Empty;
            //
            //// Read the first batch of the TcpServer response bytes.
            //Int32 bytes = stream.Read(data, 0, data.Length);
            //responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
            //Console.WriteLine("Received: {0}", responseData);

            // Close everything.
            stream.Close();
            client.Close();
        }
        catch (ArgumentNullException e)
        {
            Debug.Log("ArgumentNullException: " + e.ToString());
        }
        catch (SocketException e)
        {
            Debug.Log("SocketException:  " + e.ToString());
        }

	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
