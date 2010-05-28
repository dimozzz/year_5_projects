package gui;

import impl.query.Query;

import javax.swing.*;
import javax.swing.border.CompoundBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.BlockingQueue;

/**
 * Created by HREN_VAM.
 */
public class MainFrame extends JFrame{

    private BlockingQueue<String> queue;
    private JTextArea text;

    public MainFrame(String title, BlockingQueue<String> queue){
        super(title);

        this.queue = queue;
        
        getContentPane().setLayout(new BorderLayout());

        text = new JTextArea("ЧОЧО!1 ЖЫВТОНЕ!!1 УПЯЧКА1 УПЯЧКА11");
        JEditorPane edit = new JEditorPane();

        text.setEditable(false);
        text.setBorder(new CompoundBorder());

        JPanel p = new JPanel();
        p.setLayout(new BorderLayout());
        p.add(edit, BorderLayout.CENTER);
        JButton b = new JButton();
        b.setPreferredSize(new Dimension(80, 30));
        b.setAction(new SendAction("Send1", edit));
        p.add(b, BorderLayout.EAST);

        getContentPane().add(text, BorderLayout.CENTER);
        getContentPane().add(p, BorderLayout.SOUTH);

        setVisible(true);
        getContentPane().setSize(600, 400);

    }

    public void addText(String s){
        text.append(s + "\n");
    }

    private class SendAction extends AbstractAction{

        private JEditorPane edit;

        public SendAction(String name, JEditorPane edit){
            super(name);
            this.edit = edit;
        }

        public void actionPerformed(ActionEvent e){
            queue.offer(edit.getText());
            edit.setText("");
        }
    }
}
