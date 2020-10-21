package bgu.spl.net.srv;

public class Frame {
    private String[] input;
    private String line1;
    private String line2;
    private String line3;
    private String line4;
    private String line5;
    private String line6;
    private String line7;
    private String body="";
    public Frame(String msg){
        input = msg.split("\n");
        if(!isVaild()){
            moveAll();
        }

     for(int i1=0;i1<input.length;i1++)
        if (input[i1].equals("CONNECT")) {
            line1= "CONNECT";
            for(int i2=0;i2<input.length;i2++)
                if(input[i2].contains("accept-version:"))
                    line2 = input[i2].substring(7);
            for(int i2=0;i2<input.length;i2++)
                if(input[i2].contains("host:"))
                    line3 = input[i2].substring(5);
            for(int i2=0;i2<input.length;i2++)
                if(input[i2].contains("login:"))
                    line4 = input[i2].substring(6);
            for(int i2=0;i2<input.length;i2++)
                if(input[i2].contains("passcode:"))
                    line5 = input[i2].substring(9);
        }
        for(int i=0;i<input.length;i++)
            if (input[i].equals("SEND")) {
                line1= "SEND";
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("destination:"))
                        line2 = input[i2].substring(12);
                body = input[3];
            }
        for(int i=0;i<input.length;i++)
            if (input[0].equals("SUBSCRIBE")) {
                line1= "SUBSCRIBE";
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("destination:"))
                        line2 = input[i2].substring(12);
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("id:"))
                        line3 = input[i2].substring(3);
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("receipt:"))
                        line4 = input[i2].substring(8);
            }
        for(int i=0;i<input.length;i++)
            if (input[0].equals("UNSUBSCRIBE")) {
                line1= "UNSUBSCRIBE";
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("id:"))
                        line2 = input[i2].substring(3);
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("receipt:"))
                        line3 = input[i2].substring(8);
            }
        for(int i=0;i<input.length;i++)
            if (input[0].equals("DISCONNECT")) {
                line1= "DISCONNECT";
                for(int i2=0;i2<input.length;i2++)
                    if(input[i2].contains("receipt:"))
                        line2 = input[i2].substring(8);
            }
    }


    public String getLine2() {
        return line2;
    }
    public String getLine1() {
        return line1;
    }
    public String getLine3() {
        return line3;
    }
    public String getLine4() {
        return line4;
    }
    public String getLine5() {
        return line5;
    }
    public String getLine6() {
        return line6;
    }
    public String getLine7() {
        return line7;
    }
    public String getBody() {
        return body;
    }
    public void moveAll(){
        for (int i=0;i<input.length-1;i++)
            input[i]=input[i+1];
    }
    public boolean isVaild(){
        if(input[0].equals("CONNECT"))
            return true;
        if(input[0].equals("SEND"))
            return true;
        if(input[0].equals("SUBSCRIBE"))
            return true;
        if(input[0].equals("UNSUBSCRIBE"))
            return true;
        if(input[0].equals("DISCONNECT"))
            return true;
        return false;
    }
}
