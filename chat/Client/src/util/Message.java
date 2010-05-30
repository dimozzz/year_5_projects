package util;

public class Message {
    private final String author;
    private final String text;

    public Message(String text, String author) {
        this.text = text;
        this.author = author;
    }

    public String getAuthor() {
        return author;
    }

    public String getText() {
        return text;
    }
}
