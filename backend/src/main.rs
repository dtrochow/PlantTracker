#[cfg(not(debug_assertions))]
use actix_files::Files;
use actix_web::{get, App, HttpResponse, HttpServer, HttpRequest, Responder};
use rand::Rng;
use serde_json::json;

#[get("/api/temperature")]
async fn temperature() -> impl Responder {
    let temperature_cels = rand::thread_rng().gen_range(0..100);
    HttpResponse::Ok().json(json!({
        "temperature": temperature_cels
    }))
}

#[get("/test")]
async fn test_endpoint(req: HttpRequest) -> impl Responder {
    println!("Handling request to /test endpoint");

    if let Some(temperature_header) = req.headers().get("Temperature") {
        if let Ok(temp_str) = temperature_header.to_str() {
            println!("Received Temperature: {}", temp_str);
        } else {
            println!("Failed to parse Temperature header");
        }
    } else {
        println!("No Temperature header found");
    }

    HttpResponse::Ok().body("This is a response from the /test endpoint")
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    dotenvy::from_filename(".env.dev").expect("Failed to load .env.dev file");

    HttpServer::new(move || {
        let app = App::new()
            .service(temperature)
            .service(test_endpoint);

        // Serve static files only in production mode
        #[cfg(not(debug_assertions))]
        return app.service(Files::new(
            "/", 
            std::env::var("FRONTEND_STATIC_PATH").expect("FRONTEND_STATIC_PATH environment variable wasn't found. Specify location of bundled frontend files using FRONTEND_STATIC_PATH environment variable.")
        ).index_file("index.html"));

        #[cfg(debug_assertions)]
        return app;
    })
    .bind(("0.0.0.0", 8080))?
    .run()
    .await
}
