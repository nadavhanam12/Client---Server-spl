package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoderIMP;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocolIMP;
import bgu.spl.net.impl.newsfeed.NewsFeed;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.Server;

import java.util.function.Supplier;

public class StompServer {

    public static void main(String[] args) {
        if (args[1].equals("tpc")) {

            Server.threadPerClient(
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolIMP::new, //protocol factory
                    MessageEncoderDecoderIMP::new //message encoder decoder factory
            ).serve();
        } else {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), //port
                    StompMessagingProtocolIMP::new, //protocol factory
                    MessageEncoderDecoderIMP::new //message encoder decoder factory
            ).serve();


        }
    }

}



