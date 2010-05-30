package gui;

import util.Message;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.concurrent.BlockingQueue;

/**
 * @author Sokolov.
 */
public class MainFrame extends JFrame {

    private final JTextField editor = new JTextField();
    private final JTextArea otherMessages = new JTextArea();

    public MainFrame(String title, final BlockingQueue<String> outcomingMessages) {
        super(title);

        editor.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                outcomingMessages.add(editor.getText());
                editor.setText("");
            }
        });
        getContentPane().add(new JScrollPane(otherMessages), BorderLayout.CENTER);
        otherMessages.setEditable(false);
        getContentPane().add(editor, BorderLayout.SOUTH);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                System.out.println("MainFrame: exit");
                outcomingMessages.offer(":quit");
            }
        });
        setSize(600, 400);
        setVisible(true);                
    }

    public void publishMessage(Message message) {
        otherMessages.setText(otherMessages.getText() + "\n" + message.getAuthor() + "> " + message.getText());
    }

}
