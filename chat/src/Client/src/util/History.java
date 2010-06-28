package util;

import java.util.*;

/**
 * Created by HREN_VAM.
 */
public class History{

    private ArrayList<String> myMessages;
    private ArrayList<Message> serverMessages;
    private String myName;

    public History(String myName){
        myMessages = new ArrayList<String>();
        serverMessages = new ArrayList<Message>();
        this.myName = myName;
    }

    public void addMyMessage(String text){
        myMessages.add(text);
        System.out.println(myMessages.toString());
    }

    public void addServerMessage(Message mes){
        serverMessages.add(mes);
        Collections.sort(serverMessages);
        System.out.println(serverMessages.toString());
        if(!mes.getAuthor().equals(myName)){
            return;
        }
        for(Iterator<String> it = myMessages.iterator();it.hasNext();){
            if(it.next().equals(mes.getText())){
                it.remove();
                break;
            }
        }
    }

    public List<String> getMyMessages(){
        return Collections.unmodifiableList(myMessages);
    }

    public List<Message> getServerMessages(){
        return Collections.unmodifiableList(serverMessages);
    }

    public String getName(){
        return myName;
    }
}
