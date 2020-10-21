package bgu.spl.net.api;

import bgu.spl.net.srv.Client;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImp;
import bgu.spl.net.srv.Frame;

import java.io.IOException;

public class StompMessagingProtocolIMP<T> implements StompMessagingProtocol<T> {
    private  int connectionId;
    private String msgError;
    private  String version;
    private  String host;
    private  String name;
    private  String password;
    private  String destination;
    private  String idfromClient;
    private  String receipt;
    private  String channel;
    private  String msg;
    private boolean shouldTer=false;
    private Connections<String> connections;

    public void StompMessagingProtocolIMP(){ }
    public void start(int connectionId, Connections<String> connections){
        this.connectionId=connectionId;
        this.connections=connections;
    }

    public void process(String message) {
        if (!message.equals("m")) {
           //System.out.println("H-" + connectionId + " input protocol: " + message);
            Frame frame = new Frame(message);
            if (frame.getLine1().equals("CONNECT")) {
                if (ConnectionsImp.getInstance().isActive(connectionId)) {
                    msgError = ConnectionsImp.getInstance().MakeError("", "Logout from current user before connecting to a new one");
                    ConnectionsImp.getInstance().send(connectionId, msgError);
                    //ConnectionsImp.getInstance().closeConnection(connectionId);
                } else {
                    version = frame.getLine2();
                    host = frame.getLine3();
                    name = frame.getLine4();

                    password = frame.getLine5();
                    if (ConnectionsImp.getInstance().getConnectedClientstoHandler().containsKey(name)) {
                        msgError = ConnectionsImp.getInstance().MakeError("", "User already logged in");
                        ConnectionsImp.getInstance().send(connectionId, msgError);
                        ConnectionsImp.getInstance().closeConnection(connectionId);
                        this.Terminate();
                    } else if (ConnectionsImp.getInstance().getAllClients().containsKey(name)) {
                        Client curClient = (Client) ConnectionsImp.getInstance().getAllClients().get(name);
                        if (curClient.getPassword().equals(password)) {
                            ConnectionsImp.getInstance().activeClient(connectionId, name);
                            msg = "CONNECTED" + "\n" + version + "\n\n" + '\u0000';
                            ConnectionsImp.getInstance().send(connectionId, msg);

                        } else {
                            msgError = ConnectionsImp.getInstance().MakeError("", "Wrong password");
                            ConnectionsImp.getInstance().send(connectionId, msgError);
                            ConnectionsImp.getInstance().closeConnection(connectionId);
                            this.Terminate();
                        }
                    } else {
                        ConnectionsImp.getInstance().addClient(name, password, connectionId);
                        msg = "CONNECTED" + "\n" + version + "\n\n" + '\u0000';
                        ConnectionsImp.getInstance().send(connectionId, msg);
                    }
                }
            } else if (!ConnectionsImp.getInstance().isActive(connectionId)) {
                msgError = ConnectionsImp.getInstance().MakeError("", "U Have To Connect To A User Before Using The Server");
                ConnectionsImp.getInstance().send(connectionId, msgError);
               // ConnectionsImp.getInstance().closeConnection(connectionId);
                //this.Terminate();
            } else {

                if (frame.getLine1().equals("SUBSCRIBE")) {
                    destination = frame.getLine2();
                    idfromClient = frame.getLine3();
                    receipt = ConnectionsImp.getInstance().MakeReceipt(Integer.parseInt(frame.getLine4()));
                    name = ConnectionsImp.getInstance().getName(connectionId);
                    ConnectionsImp.getInstance().Subscribe(destination, name, Integer.parseInt(idfromClient));
                    ConnectionsImp.getInstance().send(connectionId, receipt);
                }
                if (frame.getLine1().equals("UNSUBSCRIBE")) {
                    idfromClient = frame.getLine2();
                    name = ConnectionsImp.getInstance().getName(connectionId);
                    ConnectionsImp.getInstance().UNSubscribe(name, Integer.parseInt(idfromClient));
                    msg =ConnectionsImp.getInstance().MakeReceipt(Integer.parseInt(frame.getLine3()));
                    ConnectionsImp.getInstance().send(connectionId, msg);
                }
                if (frame.getLine1().equals("SEND")) {
                    channel = frame.getLine2();
                    msg = frame.getBody();
                    //System.out.println("print from send1: "+(String)msg);
                    //System.out.println("print from send11: "+msg);

                    ConnectionsImp.getInstance().send(channel, msg);
                }
                if (frame.getLine1().equals("DISCONNECT")) {
                    receipt = ConnectionsImp.getInstance().MakeReceipt(Integer.parseInt(frame.getLine2()));
                    ConnectionsImp.getInstance().send(connectionId, receipt);
                    ConnectionsImp.getInstance().disconnect(connectionId);
                    this.Terminate();
                }

            }
        }
    }
        public boolean shouldTerminate () {
            return shouldTer;
        }
        public void Terminate () {
        shouldTer=true;
    }
        public String getDestination () {
            return destination;
        }
        public String getIdfromClient () {
            return idfromClient;
        }
        public String getReceipt () {
            return receipt;
        }
    }
