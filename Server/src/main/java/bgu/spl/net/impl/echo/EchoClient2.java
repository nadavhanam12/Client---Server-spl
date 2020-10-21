package bgu.spl.net.impl.echo;

import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class EchoClient2 {

    public static void main(String[] args) throws IOException {
        String[] input=new String[7];
        input[0]="CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:nadav\npasscode:hanam\n\n^@";
        input[1]="SUBSCRIBE\ndestination:z\nid:1\nreceipt:1\n\n^@";
        input[2]="SEND\ndestination:z\n\nnadav has added the book nadav1\nˆ@";
        input[3]="SEND\ndestination:z\n\nnadav has added the book nadav11\nˆ@";
        input[4]="UNSUBSCRIBE\nid:1\nreceipt:111\n\nˆ@";
        input[5]="DISCONNECT\nreceipt:11\n\nˆ@";
        input[6]="m";

        if (args.length == 0) {
            args = new String[]{"localhost", "llccccccl"};
        }

        if (args.length < 2) {
            System.out.println("you must supply two arguments: host, message");
            System.exit(1);
        }
        //BufferedReader and BufferedWriter automatically using UTF-8 encoding
        try (Socket sock = new Socket(args[0], 1234);
             BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
             BufferedWriter out = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()))) {
            Scanner myObj = new Scanner(System.in);
            String line="ss";
            while(!line.contains("ERROR")){
                System.out.println("-- Enter A commend for handler 2");
                String str = myObj.nextLine();
                str=input[Integer.parseInt(str)];
                System.out.println("sending message to server: "+str);
                //System.out.println(str);

                out.write(str);
                out.newLine();
                out.flush();
                line="";
                System.out.println("awaiting response");
                System.out.println("message from server: ");
                while(! line.contains("^@")){
                    line = in.readLine();
                    System.out.println(line);
                }

            }
        }
    }
}
