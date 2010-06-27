package gui;

import util.Message;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.concurrent.BlockingQueue;
import java.util.List;
import java.util.ArrayList;

import java.io.UnsupportedEncodingException;

/**
 * @author Sokolov.
 */
public class MainFrame extends JFrame {

    private final JTextField editor = new JTextField();
    private final JTextArea otherMessages = new JTextArea();
    private final JTextArea userArea = new JTextArea();
    private final List<String> users = new ArrayList<String>();

    public MainFrame(final BlockingQueue<String> outcomingMessages) {

        editor.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                String message = editor.getText(); 
                try {
                    outcomingMessages.put(new String(message.getBytes("UTF-8"), "UTF-8"));
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }catch(UnsupportedEncodingException e){
					throw new RuntimeException(e);
				}
                if (message.equals(":quit")) {
                    dispose();
                } else {
                    editor.setText("");
                }
            }
        });
        userArea.setEditable(false);
        JSplitPane panel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true);
        panel.add(new JScrollPane(otherMessages), JSplitPane.LEFT, 0);
        panel.add(new JScrollPane(userArea), JSplitPane.RIGHT, 1);
        getContentPane().add(panel, BorderLayout.CENTER);

        otherMessages.setEditable(false);
        getContentPane().add(editor, BorderLayout.SOUTH);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                try {
                    outcomingMessages.put(":quit");
                    dispose();
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
            }
        });
        setSize(600, 400);
        setVisible(true);
        panel.setDividerLocation(0.7);        
    }

    public void publishMessage(Message message) {
        otherMessages.setText(otherMessages.getText() + message.getAuthor() + "> " + message.getText() + "\n");
    }

    public synchronized void addUser(String name) {
        users.add(name);
        if (users.size() == 1)
            userArea.setText(name);
        else
            userArea.setText(userArea.getText() + "\n" + name);
    }

    public synchronized void removeUser(String name) {
        users.remove(users.indexOf(name));
        String text = "";
        for (int i = 0; i != users.size() - 1; ++i)
            text += users.get(i) + "\n";
        if (!users.isEmpty())
            text += users.get(users.size() - 1);
        userArea.setText(text);
    }

}
