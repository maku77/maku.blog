---
title: "JavaFX2 で HelloWorld（ウィンドウの表示）"
url: "/p/haq7myf"
date: "2011-10-10"
tags: ["Java"]
---

{{< image border="true" src="img-001.png" >}}

{{< code lang="java" title="Main.java" >}}
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class Main extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        Group root = new Group();
        Scene scene = new Scene(root, 400, 300, Color.GRAY);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
{{< /code >}}

