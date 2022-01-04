---
title: "JavaFX2 で CheckBox を ScrollPane に並べる"
url: "/p/j6jv8iu"
date: "2011-10-10"
tags: ["Java"]
---

{{< image border="true" src="img-001.png" >}}

{{< code lang="java" title="Main.java" >}}
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.CheckBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class Main extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    private Group createRootGroup() {
        CheckBox cb1 = new CheckBox("First");
        CheckBox cb2 = new CheckBox("Second");
        CheckBox cb3 = new CheckBox("Third");

        // Align vertically
        VBox vbox = new VBox();
        vbox.setSpacing(10);
        vbox.getChildren().add(cb1);
        vbox.getChildren().add(cb2);
        vbox.getChildren().add(cb3);

        // Add to the root
        Group root = new Group();
        root.getChildren().add(vbox);
        return root;
    }

    @Override
    public void start(Stage primaryStage) {
        Group root = createRootGroup();
        Scene scene = new Scene(root, 200, 150, Color.IVORY);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
{{< /code >}}

