package bgu.spl.net.srv;

import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

public class Client {
    private String name;
    private String password;
    private Integer IDHendler;
    private ConcurrentHashMap<String,Integer> IDSubscribe;

    public Client(String name,String password,Integer ID){
        this.name=name;
        this.password=password;
        this.IDHendler=ID;
        this.IDSubscribe=new ConcurrentHashMap<>();

    }
    public int getIDfortopic(String topic){return IDSubscribe.get(topic);}
    public String gettopicByid(int id){
        for(String it : IDSubscribe.keySet()) {
            if (IDSubscribe.get(it) == id)
                return it;
        }
        return null;
    }
    public void Disconnect(){
        IDSubscribe.clear();
    }
    public void Subscribed(String topic,int id){
        this.IDSubscribe.put(topic,id);
    }
    public void UNSubscribed(String topic,int id){
        this.IDSubscribe.remove(topic,id);
    }
    public String getName(){return name;}
    public String getPassword(){return password;}
    public Integer getID(){return IDHendler;}

    public ConcurrentHashMap<String,Integer> getIDSubscribe() {
        return IDSubscribe;
    }
}
