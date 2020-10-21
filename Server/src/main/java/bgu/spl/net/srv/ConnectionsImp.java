package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoderIMP;
import javafx.util.Pair;

import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Handler;

public class ConnectionsImp<T> implements  Connections<T>{
    private static ConnectionsImp connectionsImp=new ConnectionsImp();
    private ConcurrentHashMap<Integer,ConnectionHandler<T>> Handlers;
    private ConcurrentHashMap<String,Integer> ConnectedClientstoHandler;
    private ConcurrentHashMap<Integer,String> ConnectedHandlertoClients;
    private ConcurrentHashMap<String, Client> AllClients;
    private ConcurrentHashMap<String, ConcurrentHashMap<String,Integer>> Channels;
    private AtomicInteger ID;
    private AtomicInteger msgcounter;


    private static class SingletonHolder {
        private static ConnectionsImp instance = new ConnectionsImp();
    }
    public ConnectionsImp(){
        ConnectedClientstoHandler=new ConcurrentHashMap<>();
        ConnectedHandlertoClients=new ConcurrentHashMap<>();
        AllClients=new ConcurrentHashMap<>();
        Channels=new ConcurrentHashMap<>();
        ID=new AtomicInteger(1);
        msgcounter=new AtomicInteger(1);
        Handlers=new ConcurrentHashMap();
    }
    public static ConnectionsImp getInstance() {
        return connectionsImp;
    }
    public ConcurrentHashMap<Integer, ConnectionHandler<T>> getHandlers() {
        return Handlers;
    }
    public String getNameperconnectionid(int id){
        return ConnectedHandlertoClients.get(id);
    }
    public int getIdsubforchannel(String topic,String name){
        return AllClients.get(name).getIDfortopic(topic);
    }
    public ConcurrentHashMap<String, ConcurrentHashMap<String,Integer> > getChannels(){
        return this.Channels;
    }
    public ConcurrentHashMap<String,Client> getAllClients(){
        return this.AllClients;
    }
    public void UNSubscribe(String name, int id) {
        String topic=AllClients.get(name).gettopicByid(id);
        AllClients.get(name).UNSubscribed(topic,id);
            if(Channels.get(topic).containsKey(name))
                Channels.get(topic).remove(name);
        }
    public ConcurrentHashMap<String,Integer> getConnectedClientstoHandler(){
        return this.ConnectedClientstoHandler;
    }
    public ConcurrentHashMap<Integer,String> getConnectedHandlertoClients(){
        return this.ConnectedHandlertoClients;
    }
    public void activeClient(int id,String name){
        ConnectedClientstoHandler.put(name,id);
        ConnectedHandlertoClients.put(id,name);

    }
    public void addHandler(Integer id,ConnectionHandler<T> handler){
       Handlers.put(id,handler);
    }
    public void Subscribe(String topic,String name,int id){
        if(! this.Channels.containsKey(topic))
            this.Channels.put(topic,new ConcurrentHashMap<>());
        this.Channels.get(topic).put(name,id);
        this.AllClients.get(name).Subscribed(topic,id);

    }
    public String getName(Integer idhandler){
        return this.ConnectedHandlertoClients.get(idhandler);
    }
    public int getID(){
        return ID.getAndIncrement();
    }
    public void disconnect(int connectionId) {
        String name=ConnectedHandlertoClients.get(connectionId);
        ConnectedHandlertoClients.remove(connectionId);
        ConnectedClientstoHandler.remove(name);
        ConcurrentHashMap<String,Integer> topicstoClean= AllClients.get(name).getIDSubscribe();
        for(String it: topicstoClean.keySet())
            Channels.get(it).remove(name);
        AllClients.get(name).Disconnect();
        closeConnection(connectionId);

    }
    public void closeConnection(int connectionId)  {
        ConnectionHandler<T> HendlerToDelete=Handlers.get(connectionId);
        Handlers.remove(connectionId);
        /*
        try {
            HendlerToDelete.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        */
    }
    public void addClient(String name,String password,int id){
        Client newclient=new Client(name,password,id);
        activeClient(id,name);
        AllClients.put(name,newclient);
    }
    public int getMsgcounter(){
        return msgcounter.getAndIncrement();
    }
    public boolean send(int connectionId, T msg) {
        Handlers.get(connectionId).send(msg);
        return  true;
    }
    public String MakeMessage(int subid,int msgid,String channel,String msg){
        return("MESSAGE"+"\n"
                +"subscription:"+subid+"\n"
                +"Message-id:"+msgid+"\n"
                +"destination:"+channel+"\n\n"
                +msg+"\n"
                +'\u0000');
    }
    public void send(String channel, T msg)  {
        String NameToSend;
        int idToSend;
        int msgid=getMsgcounter();
        ConcurrentHashMap<String,Integer> ClientsToSend=Channels.get(channel);
        for(String it: ClientsToSend.keySet()){
            NameToSend=(it);
            int idTo=ClientsToSend.get(it);
            T readymsg= (T) MakeMessage(idTo,msgid,channel,(String)msg);
            idToSend=ConnectedClientstoHandler.get(NameToSend);
            send(idToSend,readymsg);
        }
    }
    public String MakeReceipt(int idreceipt){
        String msg=("RECEIPT"+"\n"
                +"receipt-id:"+idreceipt+"\n\n"
                +'\u0000');
        return msg;
    }
    public String MakeError(String receipe,String body){
        String Error;
        if(receipe!="") Error=("ERROR"+"\n"
                +"receipt-id:"+receipe+"\n\n"
                +body+"\n"
                +'\u0000');
        else
            Error=("ERROR"+"\n\n"
                    +body+"\n"
                    +'\u0000');
        return Error;
    }
    public boolean isActive(int id){return ConnectedHandlertoClients.containsKey(id);}
}
