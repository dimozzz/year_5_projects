package gui;

import util.Message;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Set;
import java.util.concurrent.BlockingQueue;

/**
 * @author Sokolov.
 */
public class MainFrame extends JFrame {

    private final JTextField editor = new JTextField();
    private final JTextArea otherMessages = new JTextArea();
    private final JTextArea users = new JTextArea();

    public MainFrame(String title, final BlockingQueue<String> outcomingMessages) {
        super(title);

        editor.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                String message = editor.getText(); 
                try {
                    outcomingMessages.put(message);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                if (message.equals(":quit")) {
                    dispose();
                } else {
                    editor.setText("");
                }
            }
        });
        users.setEditable(false);
        JSplitPane panel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, true);
        panel.add(new JScrollPane(otherMessages), JSplitPane.LEFT, 0);
        panel.add(new JScrollPane(users), JSplitPane.RIGHT, 1);
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
        users.setText(users.getText() + name + "\n");
    }

    public synchronized void removeUser(String name) {
        String allUsers = users.getText();
        int ind = allUsers.indexOf("\n" + name);
        if (ind == -1) {
            ind = allUsers.indexOf(name);
            assert(ind == 0);
            allUsers = allUsers.substring(name.length() + 1);
        } else {
            allUsers = allUsers.substring(0, ind) + allUsers.substring(ind + name.length() + 1);
        }
        users.setText(allUsers);
    }

}
